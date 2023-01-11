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
    static bool available;
    static std::mutex building;
    static std::vector<uint32_t> data;

    static void build_data();
    static void generate(int n1, int n2, int n3, int n4);
};
