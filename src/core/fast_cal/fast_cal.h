/// Klotski Engine by Dnomd343 @2024

// TODO: only copy from old implementation, the interfaces will change in future.

#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include "mover/mover.h"
#include "layer_queue.h"
#include "raw_code/raw_code.h"

#include <parallel_hashmap/phmap.h>

namespace klotski::fast_cal {

class FastCalPro {
public:
    FastCalPro() = delete;

    explicit FastCalPro(codec::RawCode raw_code);

    // ------------------------------------------------------------------------------------- //

    // TODO: add global build

    void build();

    /// Calculate a minimum-step case.
    std::optional<codec::RawCode> solve();

    /// Calculate all the maximum-step cases.
    std::vector<codec::RawCode> furthest();

    /// Calculate all the minimum-step cases.
    std::vector<codec::RawCode> solve_multi();

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

    // TODO: add invalid RawCode constexpr var

    struct data_t {
        uint64_t mask;
        codec::RawCode back;
    };

    // codec::RawCode root_;
    LayerQueue<codec::RawCode> codes_;
    phmap::flat_hash_map<codec::RawCode, data_t> cases_;

    // ------------------------------------------------------------------------------------- //
};

} // namespace klotski::fast_cal

#include "internal/fast_cal.inl"
