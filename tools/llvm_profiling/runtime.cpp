#include "runtime.h"
#include <iostream>
#include <stdint.h>
#include <stdlib.h>

#include "utils.h"
#include "llvm/IR/Instruction.h"
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <unordered_map>

static std::unordered_map<uint32_t, uint64_t> instr_map;

// For section 2
// num: the number of unique llvm::Instructions in the basic block. It is the length of keys and values.
// keys: the array of the opcodes of the llvm::Instructions
// values: the array of the counts of the llvm::Instructions
void updateInstrInfo(unsigned num, uint32_t *keys, uint32_t *values) {
    if (instr_map.empty())
        instr_map.reserve(10000);
    uint32_t key;
    uint32_t value;

    for (unsigned i = 0; i < num; i++) {
        key = keys[i];
        value = values[i];
        if (instr_map.find(key) == instr_map.end())
            instr_map.insert(std::make_pair(key, value));
        else
            instr_map[key] = instr_map[key] + value;
    }
    return;
}

// For section 2
void printOutInstrInfo() {
    // REF: https://github.com/llvm-mirror/llvm/blob/master/lib/IR/llvm::Instruction.cpp#L290-L371
    std::unordered_map<std::string, uint64_t> instype_map;
    std::cout << "====> Dynamic Instuction Count <====\n";
    for (auto &kv : instr_map) {
        std::cout << llvm::Instruction::getOpcodeName(kv.first) << "\t" << kv.second << "\n";
        instype_map[getOpcodeType(kv.first)] += kv.second;
    }
    std::cout << "\n====> Dynamic Instuction Types Count <====\n";
    for (auto &kv : instype_map) {
        std::cout << kv.first << "\t" << kv.second << "\n";
    }
    return;
}

// For section 3
// If taken is true, then a conditional branch is taken;
// If taken is false, then a conditional branch is not taken.

// branch_count[0]: branch taken number
// branch_count[1]: total branches number
uint64_t branch_count[2] = {0, 0};
void updateBranchInfo(bool taken) {
    if (taken)
        branch_count[0]++;
    branch_count[1]++;
    return;
}

// For section 3
void printOutBranchInfo() {
    std::cout << "====> Profiling Branch Bias <====\n";
    std::cout << "taken\t" << branch_count[0] << '\n';
    std::cout << "total\t" << branch_count[1] << '\n';

    branch_count[0] = 0;
    branch_count[1] = 0;
    return;
}

uint64_t memory_alloc_bytes = 0, memory_alloc_times = 0;
void updateMemoryAllocInfo(uint32_t byte_sizes) {
    memory_alloc_bytes += byte_sizes;
    memory_alloc_times += 1;
}

void printOutMemoryAllocInfo() {
    std::cout << "====> Profiling Memory Allocation <====\n";
    std::cout << "memory_alloc_bytes\t" << memory_alloc_bytes << '\n';
    std::cout << "memory_alloc_times\t" << memory_alloc_times << '\n';
    return;
}