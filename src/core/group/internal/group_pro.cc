#include <algorithm>

#include "group/group.h"

using klotski::cases::GroupPro;

using klotski::codec::CommonCode;

using klotski::cases::RangesUnion;

using klotski::cases::GROUP_PRO_SEED;

RangesUnion GroupPro::cases() const {
    auto seed = CommonCode::unsafe_create(GROUP_PRO_SEED[flat_id()]);

    // NOTE: convert as RawCode directly
    if (mirror_toward_ == 1) {
        seed = seed.to_horizontal_mirror();
    } else if (mirror_toward_ == 2) {
        seed = seed.to_vertical_mirror();
    } else if (mirror_toward_ == 3) {
        // NOTE: avoid multi convert
        seed = seed.to_vertical_mirror().to_horizontal_mirror();
    }

    auto codes = Group::extend(seed.to_raw_code(), size());

    RangesUnion data;
    for (auto raw_code : codes) {
        auto common_code = raw_code.to_common_code().unwrap();
        data[common_code >> 32].emplace_back(static_cast<uint32_t>(common_code));
    }

    for (int head = 0; head < 16; ++head) {
        std::stable_sort(data[head].begin(), data[head].end());
    }
    return data;
}
