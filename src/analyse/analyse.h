#pragma once

#include <queue>
#include <cstdint>
#include <unordered_map>

#include <set>
#include <list>
#include <unordered_set>

class Analyse {
public:
    struct analyse_t {
        uint64_t code;
        uint64_t mask;

        int step;

//        bool highlight;

//        std::vector<analyse_t*> src;
        std::list<analyse_t*> src;
//        std::set<analyse_t*> src;
//        std::unordered_set<analyse_t*> src;
    };

    struct backtrack_t {
        uint64_t code;
        std::list<backtrack_t*> next;
    };

    std::queue<analyse_t*> cache;
    std::unordered_map<uint64_t, analyse_t> cases;

    inline Core new_core();

    // TODO: backtrack for multi-codes
    void backtrack(uint64_t code);

    void start_analyse(uint64_t code);

    void new_case(uint64_t code, uint64_t mask);

};
