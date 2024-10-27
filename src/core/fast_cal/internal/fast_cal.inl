#pragma once

#include "group/group.h"

namespace klotski::fast_cal {

inline FastCal::FastCal(const codec::RawCode code)
        : seeker_({code}, group::GroupUnion::from_raw_code(code).max_group_size()) {
    const auto reserve = group::GroupUnion::from_raw_code(code).max_group_size();
    cases_.reserve(static_cast<size_t>(reserve * 1.56));
    cases_.emplace(code, info_t {0, nil}); // without mask
}

inline KLSK_INLINE bool FastCal::try_emplace(const codec::RawCode code, const uint64_t mask) {
    if (const auto match = cases_.find(code); match != cases_.end()) {
        match->second.mask |= mask; // update mask
        return false;
    }
    cases_.emplace(code, info_t {
        .mask = mask,
        .parent = seeker_.current(),
    });
    seeker_.emplace(code);
    return true;
}

inline KLSK_INLINE void FastCal::spawn_next(mover::MaskMover &mover) {
    auto curr = seeker_.current();
    mover.next_cases(curr, cases_.find(curr)->second.mask);
    seeker_.next();
}

} // namespace klotski::fast_cal
