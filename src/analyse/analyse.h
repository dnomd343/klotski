#pragma once

#include <queue>
#include <cstdint>
#include <unordered_map>

#include <set>
#include <list>

class Analyse {
public:
    struct analyse_t {
        uint64_t code;
        uint64_t mask;

        int step;
        // TODO: try set / unordered_set
//        std::list<analyse_t*> src;
        std::set<analyse_t*> src;
    };

    std::queue<analyse_t*> cache;
    std::unordered_map<uint64_t, analyse_t> cases;

    inline Core new_core();

    void start_analyse(uint64_t code);

    void new_case(uint64_t code, uint64_t mask);

};
