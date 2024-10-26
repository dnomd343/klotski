/// Klotski Engine by Dnomd343 @2024

// TODO: only copy from old implementation, the interfaces will change in future.

#pragma once

#include <queue>
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>

#include "mover/mover.h"
#include "raw_code/raw_code.h"

#include "group/group.h"

// #include <absl/container/flat_hash_map.h>

#include <ranges>

#include <parallel_hashmap/phmap.h>

using klotski::codec::RawCode;
using klotski::mover::MaskMover;

// TODO: using prime number
const uint32_t FC_MAP_RESERVE = 65536 * 8;

/// FastCal not found -> return invalid raw code
const RawCode FC_NOT_FOUND = RawCode::unsafe_create(0);

class FastCal {
public:
    typedef std::function<bool(uint64_t)> match_t;

    /// setting root code
    void set_root(const RawCode &code);
    explicit FastCal(const RawCode &code);

    /// backtrack functions
    int step_num(const RawCode &code);
    std::vector<RawCode> backtrack(const RawCode &code);

    /// BFS search functions
    void build();
    RawCode solve();
    std::vector<RawCode> furthest();
    std::vector<RawCode> solve_multi();
    RawCode target(const match_t &match);
    std::vector<RawCode> target_multi(const match_t &match);

    RawCode demo();

    /// static BFS search functions
    static std::vector<RawCode> resolve(const RawCode &start);
    static std::vector<std::vector<RawCode>> to_furthest(const RawCode &start);
    static std::vector<std::vector<RawCode>> resolve_multi(const RawCode &start);
    static std::vector<RawCode> search(const RawCode &start, const match_t &match);
    static std::vector<std::vector<RawCode>> search_multi(const RawCode &start, const match_t &match);

private:
    struct fast_cal_t {
        uint64_t code;
        uint64_t mask;
        fast_cal_t *last;
    };

    uint64_t root;
    std::queue<fast_cal_t*> cache;
    std::unordered_map<uint64_t, fast_cal_t> cases;

    inline MaskMover init(uint64_t code);
    void new_case(uint64_t code, uint64_t mask);
};

RawCode FastCal_demo(RawCode code);

namespace klotski {

template <typename T>
class LayerQueue {
public:
    LayerQueue(size_t reserve, std::initializer_list<T> first_layer);

    void emplace(T item);

    T current() const;

    void next();

    [[nodiscard]] bool is_ending() const;

    [[nodiscard]] bool is_new_layer() const;

    std::vector<T> layer_cases() const;

private:
    size_t queue_begin_ {0};
    size_t queue_end_ {0};

    size_t layer_begin_ {0};
    size_t layer_end_ {0};

    std::vector<T> data_ {};
};

} // namespace klotski

namespace klotski::fast_cal {

class FastCalPro {
public:
    FastCalPro() = delete;

    explicit FastCalPro(RawCode raw_code);

    // ------------------------------------------------------------------------------------- //

    /// Calculate the case with minimum steps.
    std::optional<RawCode> solve();

    /// Calculate the cases with maximum steps.
    std::vector<RawCode> furthest();

    /// Calculate all of the minimum-step cases.
    std::vector<RawCode> solve_multi();

    /// Calculate the first case that meets the requirement.
    std::optional<RawCode> achieve(std::function<bool(RawCode)> &&match);

    // ------------------------------------------------------------------------------------- //

private:
    // ------------------------------------------------------------------------------------- //

    /// Search next step cases and pop current.
    KLSK_INLINE void spawn_next(MaskMover &mover);

    /// Try to emplace the searched info into the cache.
    KLSK_INLINE bool try_emplace(uint64_t code, uint64_t mask);

    // ------------------------------------------------------------------------------------- //

    struct data_t {
        uint64_t mask;
        uint64_t back;
    };

    LayerQueue<uint64_t> codes_;
    phmap::flat_hash_map<uint64_t, data_t> cases_;

    // ------------------------------------------------------------------------------------- //
};

} // namespace klotski::fast_cal

#include "internal/layer_queue.inl"
#include "internal/fast_cal.inl"
