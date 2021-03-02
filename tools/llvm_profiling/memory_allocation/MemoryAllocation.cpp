#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "memory-allocation"

using namespace llvm;
using namespace std;

// extern void updateBranchInfo(bool taken);
// extern void printOutBranchInfo();

namespace {
struct MemoryAllocPass : public ModulePass {
    static char ID;
    MemoryAllocPass() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
        for (auto F = M.begin(); F != M.end(); ++F) {
            runOnFunction(*F);
        }
        Function *mainFunc = M.getFunction("main");
        if (mainFunc) {
            // void printOutMemoryAllocInfo();
            FunctionCallee prt =
                M.getOrInsertFunction("printOutMemoryAllocInfo", Type::getVoidTy(M.getContext()));
            for (auto B = mainFunc->begin(); B != mainFunc->end(); B++) {
                for (auto I = B->begin(); I != B->end(); I++) {
                    // insert at the end of main function
                    if ((std::string)I->getOpcodeName() == "ret") {
                        IRBuilder<> Builder(&*I);
                        // instrument printOutMemoryAllocInfo
                        Builder.CreateCall(prt);
                    }
                }
            }
        }
        return false;
    }

    static uint64_t getAllocaSizeInBytes(const AllocaInst *AI) {
        uint64_t ArraySize = 1;
        if (AI->isArrayAllocation()) {
            const ConstantInt *CI = dyn_cast<ConstantInt>(AI->getArraySize());
            if(!CI) return 0;
            // assert(CI && "non-constant array size");
            ArraySize = CI->getZExtValue();
        }
        Type *Ty = AI->getAllocatedType();
        uint64_t SizeInBytes = AI->getModule()->getDataLayout().getTypeAllocSize(Ty);
        return SizeInBytes * ArraySize;
    }

    bool runOnFunction(Function &F) {
        LLVMContext &context = F.getParent()->getContext();

        // void updateMemoryAllocInfo(uint64_t size);
        FunctionCallee udt = F.getParent()->getOrInsertFunction(
            "updateMemoryAllocInfo", Type::getVoidTy(context), Type::getInt32Ty(context));

        for (Function::iterator B = F.begin(), BE = F.end(); B != BE; ++B) {
            for (BasicBlock::iterator I = B->begin(), IE = B->end(); I != IE; ++I) {
                if (I->getOpcode() != llvm::Instruction::Alloca)
                    continue;
                llvm::AllocaInst *AI = dyn_cast<llvm::AllocaInst>(&*I);
                uint64_t Size = getAllocaSizeInBytes(AI);
                if(Size <= 0)
                    continue;
                vector<Value *> args;
                args.push_back(llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(F.getParent()->getContext()), Size));
                IRBuilder<> Builder(&*I);
                Builder.CreateCall(udt, args);
            }
        }
        return false;
    }
}; // end of struct TestPass
} // end of anonymous namespace

char MemoryAllocPass::ID = 0;
static RegisterPass<MemoryAllocPass> X(DEBUG_TYPE, "Memory Allocation",
                                       false /* Only looks at CFG */, false /* Analysis Pass */);

static cl::opt<std::string> InputFilename(cl::Positional, cl::desc("<filename>.bc"), cl::init(""));
static cl::opt<std::string> OutputFilename(cl::Positional,
                                           cl::desc("<filename>-instrumented.bc"), cl::init(""));

#if LLVM_VERSION_MAJOR >= 4
static ManagedStatic<LLVMContext> GlobalContext;
static LLVMContext &getGlobalContext() { return *GlobalContext; }
#endif

int main(int argc, const char *argv[]) {
    LLVMContext &Context = getGlobalContext();
    // static LLVMContext Context;
    SMDiagnostic Err;
    // Parse the command line to read the Inputfilename
    cl::ParseCommandLineOptions(argc, argv, "LLVM profiling memory allocation...\n");

    // load the input module
    std::unique_ptr<Module> M = parseIRFile(InputFilename, Err, Context);
    if (!M) {
        Err.print(argv[0], errs());
        return -1;
    }

    llvm::legacy::PassManager Passes;

    /// Transform it to SSA
    // Passes.add(llvm::createPromoteMemoryToRegisterPass());
    // Passes.add(new LoopInfoWrapperPass());

    Passes.add(new MemoryAllocPass());
    Passes.run(*M.get());

    // Write back the instrumentation info into LLVM IR
    std::error_code EC;
    std::unique_ptr<ToolOutputFile> Out(
        new ToolOutputFile(OutputFilename, EC, sys::fs::F_None));
    WriteBitcodeToFile(*M.get(), Out->os());
    Out->keep();

    return 0;
}
