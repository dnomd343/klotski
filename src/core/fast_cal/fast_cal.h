/// Klotski Engine by Dnomd343 @2024

#pragma once

#include <cstdint>
#include <functional>
#include <parallel_hashmap/phmap.h>

#include "mover/mover.h"
#include "layer_queue.h"
#include "raw_code/raw_code.h"

namespace klotski::fast_cal {

class FastCal {
public:
    FastCal() = delete;

    explicit FastCal(codec::RawCode code);

    // ------------------------------------------------------------------------------------- //

    /// Calculate a minimum-step case.
    std::optional<codec::RawCode> solve();

    /// Calculate all the minimum-step cases.
    std::vector<codec::RawCode> solve_multi();

    // ------------------------------------------------------------------------------------- //

    /// Calculate all cases.
    void build_all();

    /// Calculate all the maximum-step cases.
    std::vector<codec::RawCode> furthest();

    /// Calculate the first case that meets the requirement.
    std::optional<codec::RawCode> search(std::function<bool(codec::RawCode)> &&match);

    // ------------------------------------------------------------------------------------- //

    /// Export all group cases with different minimum-step.
    [[nodiscard]] std::vector<std::vector<codec::RawCode>> exports() const;

    /// Backtrack one of the minimum-step path from specified case.
    [[nodiscard]] std::vector<codec::RawCode> backtrack(codec::RawCode code) const;

    // ------------------------------------------------------------------------------------- //

private:
    // ------------------------------------------------------------------------------------- //

    /// Search next step cases and pop current.
    KLSK_INLINE void spawn_next(mover::MaskMover &mover);

    /// Try to emplace the searched info into the cache.
    KLSK_INLINE bool try_emplace(codec::RawCode code, uint64_t mask);

    // ------------------------------------------------------------------------------------- //

    /// Using non-existed RawCode value to represent NULL.
    static constexpr auto nil = codec::RawCode::unsafe_create(0);

    // ------------------------------------------------------------------------------------- //

    struct info_t {
        uint64_t mask;
        codec::RawCode parent;
    };

    LayerQueue<codec::RawCode> seeker_;
    phmap::flat_hash_map<codec::RawCode, info_t> cases_;

    // ------------------------------------------------------------------------------------- //
};

} // namespace klotski::fast_cal

#include "internal/fast_cal.inl"
