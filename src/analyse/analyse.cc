#include <queue>
#include "core.h"
#include "analyse.h"

#include <list>
#include <iostream>
#include "raw_code.h"

Core Analyse::new_core() {
    return Core(
        [this](auto &&code, auto &&mask) { // lambda as function pointer
            new_case(std::forward<decltype(code)>(code), std::forward<decltype(mask)>(mask));
        }
    );
}

void Analyse::backtrack(uint64_t code) {

    // backtrack start at code
    std::cout << "start backtrack" << std::endl;

//    std::cout << RawCode(code).dump_case() << std::endl;
//    std::cout << "src size: " << cases[code].src.size() << std::endl;
//
//    auto last_1 = cases[code].src.front();
//    std::cout << RawCode(last_1->code).dump_case() << std::endl;
//    std::cout << "src size: " << last_1->src.size() << std::endl;


    std::unordered_map<uint64_t, backtrack_t> track_data;

//    std::vector<backtrack_t*> track_data;

    std::queue<analyse_t*> track_cache;

    std::vector<std::vector<backtrack_t*> > layer;

    // TODO: confirm that code exist

    track_cache.emplace(&cases[code]);

    track_data.emplace(code, backtrack_t {
        .code = code,
        .layer_num = track_cache.front()->step,
        .layer_index = 0, // TODO: pay attention to multi-code
        .next = std::list<backtrack_t*>{}, // without next cases
    });

    layer.resize(track_cache.front()->step + 1);
    layer[track_cache.front()->step].emplace_back(&track_data[code]);

    backtrack_t *root;
    for (;;) {

        if (track_cache.front()->src.empty()) {
            root = &track_data[track_cache.front()->code];
            break;
        }

        for (auto t : track_cache.front()->src) {

            auto current = track_data.find(t->code);

            if (current != track_data.end()) { // already exist
                // link
                current->second.next.emplace_back(
                    &track_data[track_cache.front()->code]
                );
            } else { // insert new case
                track_cache.emplace(t);
                track_data.emplace(t->code, backtrack_t {
                    .code = t->code,
                    .layer_num = t->step,
                    .layer_index = (uint32_t)layer[t->step].size(),
                    .next = std::list<backtrack_t*>{&track_data[track_cache.front()->code]},
                });

                layer[t->step].emplace_back(&track_data[t->code]);

            }

        }

        track_cache.pop();

    }

    std::cout << "size: " << track_data.size() << std::endl;

    std::cout << "Root" << std::endl;
    std::cout << RawCode(root->code).dump_case() << std::endl;
//
//    std::queue<backtrack_t*> t_cache;
//    t_cache.emplace(root);
//
//    while (!t_cache.empty()) {
//
//        for (auto t : t_cache.front()->next) {
//            std::cout << "layer " << t->layer_num << std::endl;
//            std::cout << RawCode(t->code).dump_case() << std::endl;
//            t_cache.emplace(t);
//        }
//
//        t_cache.pop();
//
//    }

    for (auto l : layer) {
        std::cout << "-----------------------" << std::endl;
        std::cout << "layer " << l.front()->layer_num;
        std::cout << " -> size = " << l.size() << std::endl;

        for (auto c : l) {
            std::cout << "(" << c->layer_num << ", " << c->layer_index << ") -> ";
            for (auto t : c->next) {
                std::cout << "(" << t->layer_num << ", " << t->layer_index << ") ";
            }
            std::cout << std::endl;

            std::cout << RawCode(c->code).dump_case() << std::endl;

        }

    }

}

void Analyse::start_analyse(uint64_t code) {

    auto core = new_core();

    cases.empty();
    cache.empty();

    cases.reserve(65536);

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = 0,
        .step = 0,
//        .highlight = false,
//        .src = std::move(std::list<analyse_t*>{}),
    }).first->second);

    while (!cache.empty()) {

        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    std::cout << "size: " << cases.size() << std::endl;

}

void Analyse::new_case(uint64_t code, uint64_t mask) {


//    auto temp = cases.emplace(code, analyse_t {
//        .code = code,
//        .mask = mask,
//        .step = cache.front()->step + 1,
//        .src = std::move(std::list<analyse_t*>{cache.front()})
//    });
//
//    if (temp.second) {
//        cache.emplace(&temp.first->second);
//    } else {
//        temp.first->second.mask |= mask;
//
//        if (temp.first->second.step != cache.front()->step) {
//            temp.first->second.src.push_back(cache.front());
//        }
//
//    }

    auto current = cases.find(code);
    if (current != cases.end()) { // find existed case
        current->second.mask |= mask; // update mask info

        if (current->second.step != cache.front()->step) {
            current->second.src.push_back(cache.front());
        }

        return;
    }

    cache.emplace(&cases.emplace(code, analyse_t {
        .code = code,
        .mask = mask,
        .step = cache.front()->step + 1,
//        .highlight = false,
        .src = std::move(std::list<analyse_t*>{cache.front()}),
    }).first->second);

}
