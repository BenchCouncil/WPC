#pragma once
#include "llvm/IR/Instruction.h"

const char *getOpcodeType(unsigned OpCode) {
    switch (OpCode) {
    // Terminators
    case llvm::Instruction::Ret:
    case llvm::Instruction::Br:
    case llvm::Instruction::Switch:
    case llvm::Instruction::IndirectBr:
    case llvm::Instruction::Invoke:
    case llvm::Instruction::Resume:
    case llvm::Instruction::Unreachable:
    case llvm::Instruction::CleanupRet:
    case llvm::Instruction::CatchRet:
    case llvm::Instruction::CatchPad:
    case llvm::Instruction::CatchSwitch:
    case llvm::Instruction::CallBr:
    case llvm::Instruction::Call:
        return "branch";

    // Floating-point Operators...
    case llvm::Instruction::FNeg:
    case llvm::Instruction::FAdd:
    case llvm::Instruction::FSub:
    case llvm::Instruction::FMul:
    case llvm::Instruction::FDiv:
    case llvm::Instruction::FRem:
    case llvm::Instruction::FCmp:
    case llvm::Instruction::FPTrunc:
    case llvm::Instruction::FPExt:
    case llvm::Instruction::UIToFP:
    case llvm::Instruction::SIToFP:
        return "fp";

    // Integer operators...
    case llvm::Instruction::Add:
    case llvm::Instruction::Sub:
    case llvm::Instruction::Mul:
    case llvm::Instruction::UDiv:
    case llvm::Instruction::SDiv:
    case llvm::Instruction::URem:
    case llvm::Instruction::SRem:
    // Logical operators...
    case llvm::Instruction::And:
    case llvm::Instruction::Or:
    case llvm::Instruction::Xor:
    case llvm::Instruction::ICmp:
    case llvm::Instruction::Trunc:
    case llvm::Instruction::ZExt:
    case llvm::Instruction::SExt:
    case llvm::Instruction::FPToUI:
    case llvm::Instruction::FPToSI:
    case llvm::Instruction::IntToPtr:
    case llvm::Instruction::PtrToInt:
    case llvm::Instruction::BitCast:
    case llvm::Instruction::AddrSpaceCast:
    case llvm::Instruction::Shl:
    case llvm::Instruction::LShr:
    case llvm::Instruction::AShr:
    case llvm::Instruction::GetElementPtr:
        return "int";

    // Memory instructions...
    case llvm::Instruction::Load:
        return "load";

    case llvm::Instruction::Store:
        return "store";

    case llvm::Instruction::Fence:
    case llvm::Instruction::AtomicRMW:
    case llvm::Instruction::AtomicCmpXchg:
    case llvm::Instruction::Alloca:
    case llvm::Instruction::PHI:
    case llvm::Instruction::Select:
    case llvm::Instruction::VAArg:
    case llvm::Instruction::ExtractElement:
    case llvm::Instruction::InsertElement:
    case llvm::Instruction::ShuffleVector:
    case llvm::Instruction::ExtractValue:
    case llvm::Instruction::InsertValue:
    case llvm::Instruction::LandingPad:
    case llvm::Instruction::CleanupPad:
        return "unknown";
    default:
        return "<Invalid operator>";
    }
}
