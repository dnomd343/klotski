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

//        std::vector<analyse_t*> src;
        std::list<analyse_t*> src;
//        std::set<analyse_t*> src;
//        std::unordered_set<analyse_t*> src;
    };

    std::queue<analyse_t*> cache;
    std::unordered_map<uint64_t, analyse_t> cases;

    inline Core new_core();

    void start_analyse(uint64_t code);

    void new_case(uint64_t code, uint64_t mask);

};
