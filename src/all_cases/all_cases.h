#pragma once

#include <mutex>
#include <vector>
#include <cstdint>
#include "basic_ranges.h"

class AllCases : public BasicRanges {
public:
    static void build();
    static enum Status status();
    static const std::vector<uint32_t> (&fetch())[16];

private:
    static bool available;
    static std::mutex building;
    static std::vector<uint32_t> data[16];

    static void build_data();
};
