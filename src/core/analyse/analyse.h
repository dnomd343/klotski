/// Klotski Engine by Dnomd343 @2024

// TODO: only copy from old implementation, the interfaces will change in future.

#pragma once

#include <list>
#include <queue>
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "mover/mover.h"
#include "raw_code/raw_code.h"

namespace klotski {

// TODO: try double or 4-times size
const uint32_t ANY_MAP_RESERVE = 65536;

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
    std::unordered_map<uint64_t, analyse_t> cases;

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

} // namespace klotski
