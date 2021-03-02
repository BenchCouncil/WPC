#include <iostream>
#include <stdint.h>
#include <stdlib.h>

extern "C" {
// branch
void initBranchInfo();
void updateBranchInfo(bool taken);
void printOutBranchInfo();

// instructions
void initInstrInfo();
void updateInstrInfo(unsigned num, uint32_t *keys, uint32_t *values);
void printOutInstrInfo();

// memory alloca
void updateMemoryAllocInfo(uint32_t byte_size);
void printOutMemoryAllocInfo();
}
