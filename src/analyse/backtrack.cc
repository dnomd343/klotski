#include "analyse.h"

#include <iostream>

#include <set>
#include <unordered_set>

#include <algorithm>

#include "common_code.h"

// TODO: using const RawCode& instead of uint64_t

//namespace std {
//    template<>
//    struct hash<Analyse::backtrack_t> {
//        std::size_t operator()(const Analyse::backtrack_t &b) const {
//            std::cout << "get hash: " << b.code << std::endl;
//            return std::hash<uint64_t>()(b.code);
//        }
//    };
//
//    template<>
//    struct equal_to<Analyse::backtrack_t> {
//        bool operator()(const Analyse::backtrack_t &b1, const Analyse::backtrack_t &b2) const {
//            std::cout << "get eq: " << b1.code << " ? " << b2.code << std::endl;
//            return b1.code == b2.code;
//        }
//    };
//}

void Analyse::backtrack_demo(const std::vector<uint64_t> &codes) {


    std::vector<std::vector<analyse_t*>> todos;

    for (const auto &code : codes) {

        auto c = cases.find(code);

        if (c == cases.end()) {
            // TODO: invalid input
            return;
        }

        if (c->second.step >= todos.size()) {
            todos.resize(c->second.step + 1);
        }

        todos[c->second.step].emplace_back(&c->second);

    }

    std::reverse(todos.begin(), todos.end());


    struct cache_t {
        analyse_t *a;
        backtrack_t *b;
    };

    std::queue<cache_t> track_cache;
    std::vector<std::unordered_map<uint64_t, backtrack_t>> track_data;

//    track_data.resize(82); // TODO: setting as (max steps + 1)

    track_data.resize(todos.size());

    // TODO: for diff layer cases, layer_num from big to small -> if found then skip search

    for (const auto &todo : todos) {

        if (todo.empty()) {
            continue;
        }

        /// clear track cache
        std::queue<cache_t>{}.swap(track_cache);

        for (const auto c : todo) {

            // TODO: if c already exist -> skip
            if (track_data[c->step].find(c->code) == track_data[c->step].end()) {

                track_cache.emplace(cache_t{
                    .a = c,
                    .b = &track_data[c->step].emplace(c->code, backtrack_t{
                        .code = c->code,
                        .layer_num = c->step,
                        .last = std::list<backtrack_t *>{},
                        .next = std::list<backtrack_t *>{},
                    }).first->second,
                });
            }

        }


        while (!track_cache.empty()) {
            /// handle first element and pop it
            auto curr = track_cache.front();
            for (auto src : curr.a->src) { // traverse src cases of current node
                auto t_src = track_data[src->step].find(src->code);
                if (t_src != track_data[src->step].end()) { // match src case
                    /// link (curr.b) and (t_src->second)
                    t_src->second.next.emplace_back(curr.b);
                    curr.b->last.emplace_back(&t_src->second);
                } else { // src case not found
                    /// link (curr.b) and (t_src_new->second)
                    auto t_src_new = track_data[src->step].emplace(src->code, backtrack_t {
                        .code = src->code,
                        .layer_num = src->step,
                        .last = std::list<backtrack_t*>{},
                        .next = std::list<backtrack_t*>{curr.b}, // link to curr.b
                    }).first;
                    curr.b->last.emplace_back(&t_src_new->second);
                    /// insert into working queue
                    track_cache.emplace(cache_t {
                        .a = src,
                        .b = &t_src_new->second,
                    });
                }
            }
            track_cache.pop();
        }


    }



    for (uint32_t i = 0; i < track_data.size(); ++i) {

        const auto &ly = track_data[i];

        std::cout << std::endl;
        std::cout << "----------------------------------";
        std::cout << " layer " << i << " ";
        std::cout << "----------------------------------" << std::endl;

        for (const auto &c : ly) {
            for (const auto &l : c.second.last) {
                std::cout << l->code << " ";
            }
            std::cout << " <- [" << c.second.code << "] -> ";
            for (const auto &n : c.second.next) {
                std::cout << n->code << " ";
            }
            std::cout << std::endl;
        }

    }


}
