#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include "basic_ranges.h"

const uint32_t ALL_CASES_SIZE[16] = {
    2942906, 2260392, 2942906, 0,
    2322050, 1876945, 2322050, 0,
    2322050, 1876945, 2322050, 0,
    2942906, 2260392, 2942906, 0,
};

class AllCases : public BasicRanges {
public:
    static void build();
    static enum Status status();
    static const std::vector<uint32_t> (&fetch())[16];

//    static int check_case(uint32_t head, uint32_t range);

private:
    static bool available;
    static std::mutex building;
    static std::vector<uint32_t> data[16];

    static void build_data();
//    static int check_case(uint32_t head, uint32_t range);
};
