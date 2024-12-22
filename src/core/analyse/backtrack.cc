#include <algorithm>
#include "analyse.h"

using klotski::Analyse;
using klotski::codec::RawCode;

Analyse::track_data_t Analyse::backtrack(const std::vector<RawCode> &codes) {
    /// codes pre-check and sort by steps
    std::vector<std::vector<analyse_t*>> todos;
    for (const auto &code : codes) {
        auto c = cases.find((uint64_t)code);
        if (c == cases.end()) { // invalid input
            return track_data_t{}; // return empty data
        }
        if (c->second.step >= todos.size()) {
            todos.resize(c->second.step + 1); // enlarge schedule list
        }
        todos[c->second.step].emplace_back(&c->second);
    }
    std::reverse(todos.begin(), todos.end()); // backtrack start from further layer

    struct cache_t {
        analyse_t *a;
        track_t *b;
    };
    std::queue<cache_t> track_cache;
    track_data_t track_data(todos.size());
    /// start backtrack process
    for (const auto &todo : todos) {
        if (todo.empty()) {
            continue; // without scheduled cases
        }
        std::queue<cache_t>{}.swap(track_cache); // clear track cache
        for (const auto c : todo) {
            /// case already exist -> skip its backtrack
            if (track_data[c->step].find(c->code) == track_data[c->step].end()) {
                track_cache.emplace(cache_t{
                    .a = c,
                    .b = &track_data[c->step].emplace(c->code, track_t{
                        .code = c->code,
                        .layer_num = c->step,
                        .last = std::list<track_t*>{}, // without parent node
                        .next = std::list<track_t*>{}, // without sub node
                    }).first->second,
                });
            }
        }
        /// backtrack until root case
        while (!track_cache.empty()) {
            auto curr = track_cache.front(); // handle first element
            for (auto src : curr.a->src) { // traverse src cases of current node
                auto t_src = track_data[src->step].find(src->code);
                if (t_src != track_data[src->step].end()) { // match src case
                    /// link (curr.b) and (t_src->second)
                    t_src->second.next.emplace_back(curr.b);
                    curr.b->last.emplace_back(&t_src->second);
                } else { // src case not found
                    /// link (curr.b) and (t_src_new->second)
                    auto t_src_new = track_data[src->step].emplace(src->code, track_t {
                        .code = src->code,
                        .layer_num = src->step,
                        .last = std::list<track_t*>{},
                        .next = std::list<track_t*>{curr.b}, // link to curr.b
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
    return track_data;
}
