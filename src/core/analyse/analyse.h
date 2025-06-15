/// Klotski Engine by Dnomd343 @2024

// TODO: only copy from old implementation, the interfaces will change in future.

#pragma once

#include <list>
#include <queue>
#include <cstdint>
#include <functional>
#include <print>
#include <unordered_map>

#include "mover/mover.h"
#include "raw_code/raw_code.h"

#include "group/group.h"
#include "layer_queue.h"
#include <parallel_hashmap/phmap.h>

namespace klotski {

// TODO: try double or 4-times size
const uint32_t ANY_MAP_RESERVE = 65537; // non-prime numbers cause performance issue of libc++

// TODO: Analyse enter klotski namespace later
using namespace klotski;

class Analyse {
public:
    typedef std::function<bool(uint64_t)> match_t;

    /// setting root code
    void set_root(const codec::RawCode &code);
    explicit Analyse(const codec::RawCode &code);

    /// BFS search functions
    void build();
    std::vector<codec::RawCode> build_resolve();
    // TODO: build_furthest
    std::vector<codec::RawCode> build_until(const match_t &match);

    /// analysed layer export
    // TODO: ending point search
    std::vector<std::vector<codec::RawCode>> layer_export();
    std::vector<codec::RawCode> layer_export(uint32_t layer_num);

private:
    struct analyse_t {
        uint64_t code;
        uint64_t mask;
        uint32_t step;
        std::list<analyse_t*> src;
    };

    uint64_t root;
    std::queue<analyse_t*> cache;
    std::unordered_map<uint64_t, analyse_t> cases; // addr of analyse_t will be stable

    inline mover::MaskMover init(uint64_t code);
    void new_case(uint64_t code, uint64_t mask);

/// backtrack definitions
public:
    struct track_t {
        // TODO: try using RawCode
        uint64_t code;
        uint32_t layer_num;
        std::list<track_t*> last;
        std::list<track_t*> next;
    };
    // TODO: try using RawCode
    typedef std::vector<std::unordered_map<uint64_t, track_t>> track_data_t;

    // TODO: using RawCode instead of uint64_t
    track_data_t backtrack(const std::vector<codec::RawCode> &codes);

    // TODO: RawCode enable `hash` and `equal_to` trait in namespace std
};

namespace analyse {

class AnalysePro {
public:
    AnalysePro() = delete;

    explicit AnalysePro(codec::RawCode code) : seeker_({code}, group::GroupUnion::from_raw_code(code).max_group_size()) {
        const auto reserve = group::GroupUnion::from_raw_code(code).max_group_size();
        cases_.reserve(static_cast<size_t>(reserve * 1.56));
        cases_.emplace(code, info_t {0, 0, {}});
    }

    void build_all();

private:
    KLSK_INLINE void spawn_next(mover::MaskMover &mover) {
        auto curr = seeker_.current();
        mover.next_cases(curr, cases_.find(curr)->second.mask);
        seeker_.next();
    }

    KLSK_INLINE bool try_emplace(codec::RawCode code, uint64_t mask) {
        if (const auto match = cases_.find(code); match != cases_.end()) {

            // if (seeker_.layer_num() - 2 > match->second.step) {
            //     std::println("error!!!");
            // }

            // std::println("curr: {}, next: {}", seeker_.layer_num() - 2, match->second.step);

            if (seeker_.layer_num() - 2 + 1 == match->second.step) {
                match->second.mask |= mask; // update mask
                match->second.src.emplace_back(seeker_.current());
            }

            return false;
        }
        cases_.emplace(code, info_t {
            .mask = mask,
            .step = static_cast<int>(seeker_.layer_num() - 2 + 1),
            .src = {seeker_.current()}
        });
        seeker_.emplace(code);
        return true;
    }

    struct info_t {
        uint64_t mask;
        int step;
        std::vector<codec::RawCode> src;
    };

    LayerQueuePro<codec::RawCode> seeker_;
    phmap::flat_hash_map<codec::RawCode, info_t> cases_;
};

} // namespace analyse

} // namespace klotski
