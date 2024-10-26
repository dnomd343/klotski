#pragma once

namespace klotski::fast_cal {

inline FCDemo::FCDemo(RawCode raw_code) : codes_(cases::GroupUnion::from_raw_code(raw_code).max_group_size(), {raw_code.unwrap()}) {
//    auto reserve = cases::GroupUnion::from_raw_code(raw_code).max_group_size();
    cases_.reserve(static_cast<size_t>(25955 * 1.56));
//    cases_.reserve(static_cast<size_t>(reserve * 1.56));
    cases_.emplace(raw_code, data_t {0, 0}); // without mask
}

} // namespace klotski::fast_cal
