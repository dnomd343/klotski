#pragma once

#include <mutex>
#include <vector>
#include <cstdint>

class BasicRanges {
public:

    enum Status {
        NO_INIT,
        BUILDING,
        AVAILABLE,
    };

    static void build_basic_ranges();

    static enum Status basic_ranges_status();

    static const std::vector<uint32_t>* get_basic_ranges();

private:

    static bool basic_ranges_available;

    static std::mutex basic_ranges_building;

    static std::vector<uint32_t> basic_ranges;

    static void generate_ranges(int n1, int n2, int n3, int n4);

};
