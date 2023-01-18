#pragma once

#include <queue>
#include <cstdint>
#include <unordered_map>

#include <vector>

class FastCal {
public:

    explicit FastCal(uint64_t code) : root(code) {}

    // search resolve
    // search all min-step resolve
    // search the furthest cases
    // search target by code
    // search target by lambda

    /// xxx_multi only search until same layer

//    std::vector<uint64_t> solve(uint64_t code);
//    std::vector<uint64_t> solve();
    uint64_t solve();
//    solve_multi

//    std::vector<uint64_t> target(const std::function<bool(uint64_t)> &match);
    uint64_t target(const std::function<bool(uint64_t)> &match);

    std::vector<uint64_t> backtrack(uint64_t code);

    // TODO: continue search process? -> reuse exist data (ensure working code not changed)

    // TODO: static search functions

    const static auto NOT_FOUND = (uint64_t)0;

private:
    struct fast_cal_t {
        uint64_t code;
        uint64_t mask;
        fast_cal_t *last;
    };


    uint64_t root;

    std::queue<fast_cal_t*> cache;
    std::unordered_map<uint64_t, fast_cal_t> cases;

    Core import_core();

    void new_case(uint64_t code, uint64_t mask);

};
