#pragma once

namespace klotski::fast_cal {

inline FastCalPro::FastCalPro(codec::RawCode raw_code) : codes_({raw_code}, cases::GroupUnion::from_raw_code(raw_code).max_group_size()) {
    auto reserve = cases::GroupUnion::from_raw_code(raw_code).max_group_size();
    // cases_.reserve(static_cast<size_t>(25955 * 1.56));
    cases_.reserve(static_cast<size_t>(reserve * 1.56));
    cases_.emplace(raw_code, data_t {0, 0}); // without mask
}

inline KLSK_INLINE bool FastCalPro::try_emplace(uint64_t code, uint64_t mask) {
    if (const auto match = cases_.find(code); match != cases_.end()) {
        match->second.mask |= mask; // update mask
        return false;
    }
    cases_.emplace(code, data_t {
        .mask = mask,
        .back = codes_.current().unwrap(),
    });
    codes_.emplace(codec::RawCode::unsafe_create(code));
    return true;
}

inline KLSK_INLINE void FastCalPro::spawn_next(mover::MaskMover &mover) {
    auto curr = codes_.current();
    mover.next_cases(curr.unwrap(), cases_.find(curr.unwrap())->second.mask);
    codes_.next();
}

} // namespace klotski::fast_cal
