#pragma once

#include <list>
#include <queue>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include "core.h"

const uint32_t ANY_MAP_RESERVE = 65536;

class Analyse {
public:
    typedef std::function<bool(uint64_t)> match_t;

    void build(uint64_t code);
    std::vector<uint64_t> build_until(uint64_t code, const match_t &match);

private:
    struct analyse_t {
        uint64_t code;
        uint64_t mask;
        uint32_t step;
        std::list<analyse_t*> src;
    };

    uint64_t root;
    std::queue<analyse_t*> cache;
    std::unordered_map<uint64_t, analyse_t> cases;

    inline Core init(uint64_t code);
    void new_case(uint64_t code, uint64_t mask);


    /// backtrack definitions

//    struct backtrack_t {
//        uint64_t code;
//        uint32_t layer_num;
//        uint32_t layer_index;
//        std::list<backtrack_t*> next;
//    };

    // TODO: backtrack for multi-codes
//    void backtrack(uint64_t code);
//    void backtrack(const std::vector<uint64_t> &raw_code_list);

};
