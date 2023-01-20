#include "analyse.h"

#include <iostream>

#include <set>
#include <unordered_set>

#include "common_code.h"

// TODO: using const RawCode& instead of uint64_t

namespace std {
    template<>
    struct hash<Analyse::backtrack_t> {
        std::size_t operator()(const Analyse::backtrack_t &b) const {
            std::cout << "get hash: " << b.code << std::endl;
            return std::hash<uint64_t>()(b.code);
        }
    };

    template<>
    struct equal_to<Analyse::backtrack_t> {
        bool operator()(const Analyse::backtrack_t &b1, const Analyse::backtrack_t &b2) const {
            std::cout << "get eq: " << b1.code << " ? " << b2.code << std::endl;
            return b1.code == b2.code;
        }
    };
}

void Analyse::backtrack_demo(uint64_t code) {

    // TODO: confirm code exist
    std::cout << cases[code].step << std::endl;

    // TODO: only test for now
//    uint32_t layer_size = cases[code].step + 1; // 81 steps -> 0 ~ 81 -> size = 82

    std::vector<std::unordered_map<uint64_t, backtrack_t>> track_data;

    /// init process -> insert backtrack begin cases

    track_data.resize(82); // TODO: setting as (max steps + 1)

    struct inner_t {
        analyse_t *a;
        backtrack_t *b;
    };

    std::queue<inner_t> track_cache;

    auto rt = track_data[81].emplace(code, backtrack_t {
        .code = code,
        .layer_num = cases[code].step,
        .last = std::list<backtrack_t*>{},
        .next = std::list<backtrack_t*>{},
    });

    /// start backtrack

    track_cache.emplace(inner_t {
        .a = &cases[code],
        .b = &rt.first->second,
    });

    while (!track_cache.front().a->src.empty()) {

        auto curr = track_cache.front();

        for (auto src : curr.a->src) {

            auto find_ret = track_data[src->step].find(src->code);

            if (find_ret != track_data[src->step].end()) { // found

                find_ret->second.next.emplace_back(curr.b);

                curr.b->last.emplace_back(&find_ret->second);

            } else { // not found

//                std::cout << "new: " << src->code << std::endl;

                auto ret = track_data[src->step].emplace(src->code, backtrack_t {
                    .code = src->code,
                    .layer_num = src->step,
                    .last = std::list<backtrack_t*>{},
                    .next = std::list<backtrack_t*>{curr.b},
                });

                curr.b->last.emplace_back(&ret.first->second);

                track_cache.emplace(inner_t {
                    .a = src,
                    .b = &ret.first->second,
                });

            }

        }

        track_cache.pop();

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

//        std::cout << l.size() << std::endl;
    }


}
