#include <algorithm>

#include "group/group.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::group::Group;
using klotski::cases::RangesUnion;

using klotski::group::GROUP_DATA;
using klotski::group::PATTERN_DATA;

RangesUnion Group::cases() const {

    // TODO: add white list for single-group unions
    //       return GroupUnion::cases directly

    auto seed = CommonCode::unsafe_create(PATTERN_DATA[flat_id()] >> 23);

    // NOTE: convert as RawCode directly
    if (toward_ == Toward::B) {
        seed = seed.to_horizontal_mirror();
    } else if (toward_ == Toward::C) {
        seed = seed.to_vertical_mirror();
    } else if (toward_ == Toward::D) {
        // NOTE: avoid multi convert
        seed = seed.to_vertical_mirror().to_horizontal_mirror();
    }

    auto codes = Group_extend(seed.to_raw_code(), size());

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

static std::unordered_map<uint64_t, Group> build_map_data() {
    // NOTE: using CommonCode as map key
    std::unordered_map<uint64_t, Group> data;
    data.reserve(GROUP_DATA.size());
    for (auto raw : GROUP_DATA) {
        uint32_t type_id = (raw >> 12) & 0b11111111;
        uint32_t pattern_id = (raw >> 2) & 0b1111111111;
        uint32_t toward = raw & 0b11;
        auto seed = CommonCode::unsafe_create(raw >> 20).unwrap();
        auto group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward);
        data.emplace(seed, group);
    }
    return data;
}

Group Group::from_raw_code(codec::RawCode raw_code) {
    static auto map_data = build_map_data();

    auto raw_codes = Group_extend(raw_code);
    auto common_codes = raw_codes | std::views::transform([](const RawCode r) {
        return r.to_common_code();
    }) | std::ranges::to<std::vector>(); // TODO: search min_element directly
    auto seed = std::min_element(common_codes.begin(), common_codes.end());

    return map_data.at(seed->unwrap());
}
