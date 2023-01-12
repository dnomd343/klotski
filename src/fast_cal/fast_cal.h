#pragma once

#include <queue>
#include <cstdint>
#include <unordered_map>

class FastCal {
public:
    struct fast_cal_t {
        uint64_t code;
        uint64_t mask;
        fast_cal_t *last;
    };

    std::queue<fast_cal_t*> cache;
    std::unordered_map<uint64_t, fast_cal_t> cases;

    void fast_cal(uint64_t code);

    void new_case(uint64_t code, uint64_t mask);

};
