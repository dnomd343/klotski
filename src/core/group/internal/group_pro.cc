#include <algorithm>

#include "group/group.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::cases::GroupPro;
using klotski::cases::RangesUnion;

using klotski::cases::GROUP_PRO_SEED;
using klotski::cases::GROUP_SEED_MAP;

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

static std::unordered_map<uint64_t, GroupPro> build_map_data() {
    // NOTE: using CommonCode as map key
    std::unordered_map<uint64_t, GroupPro> data;
    data.reserve(GROUP_SEED_MAP.size());
    for (auto raw : GROUP_SEED_MAP) {
        uint32_t type_id = (raw >> 48) & 0b11111111;
        uint32_t pattern_id = (raw >> 38) & 0b1111111111;
        uint32_t toward = (raw >> 36) & 0b11;
        auto seed = CommonCode::unsafe_create(raw & (uint64_t)0xFFFFFFFFF).unwrap();
        auto group = GroupPro::unsafe_create(type_id, pattern_id, toward);
        data.emplace(seed, group);
    }
    return data;
}

GroupPro GroupPro::from_raw_code(codec::RawCode raw_code) {
    static auto map_data = build_map_data();

    auto raw_codes = Group::extend(raw_code);
    auto common_codes = raw_codes | std::views::transform([](const RawCode r) {
        return r.to_common_code();
    }) | std::ranges::to<std::vector>(); // TODO: search min_element directly
    auto seed = std::min_element(common_codes.begin(), common_codes.end());

    return map_data.at(seed->unwrap());
}
