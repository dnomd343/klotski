#pragma once

#include <queue>
#include <cstdint>
#include <unordered_map>

#include <list>
#include <set>
#include <unordered_set>

class Analyse {
public:
    struct analyse_t {
        uint64_t code;
        uint64_t mask;
        uint32_t step;

        std::list<analyse_t*> src;
//        std::vector<analyse_t*> src;
//        std::set<analyse_t*> src;
//        std::unordered_set<analyse_t*> src;
    };

//    struct backtrack_t {
//        uint64_t code;
//        uint32_t layer_num;
//        uint32_t layer_index;
//        std::list<backtrack_t*> next;
//    };

    std::queue<analyse_t*> cache;
    std::unordered_map<uint64_t, analyse_t> cases;

//    inline Core new_core();

    // TODO: backtrack for multi-codes
//    void backtrack(uint64_t code);
//    void backtrack(const std::vector<uint64_t> &raw_code_list);

    void build(uint64_t code);

//    inline Core init(uint64_t code);
    inline Core init();

    void new_case(uint64_t code, uint64_t mask);

};
