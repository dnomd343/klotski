#include <iostream>
#include "raw_code.h"

void next_step(uint64_t raw_code, uint64_t mask) {

    std::cout << RawCode(raw_code).dump_case();
    printf("mask -> %016lX\n", mask);

}
