#pragma once

#include <mutex>
#include <vector>
#include <cstdint>

const uint32_t BASIC_RANGES_SIZE = 7311921;

class BasicRanges {
public:
    enum Status {
        NO_INIT,
        BUILDING,
        AVAILABLE,
    };
    static void build();
    static enum Status status();
    static const std::vector<uint32_t>* fetch();

private:
    struct generate_t {
        int n1; // number of `00`
        int n2; // number of `01`
        int n3; // number of `10`
        int n4; // number of `11`
    };

    static bool available;
    static std::mutex building;
    static std::vector<uint32_t> data;

    static void build_data();
    static void sort_data(std::vector<int> &flags, std::vector<uint32_t> &raw);
    static void generate(std::vector<uint32_t> &release, generate_t info);
};
