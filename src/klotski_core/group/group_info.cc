#include <algorithm>
#include "group.h"
#include "type_id.h"

#include <iostream>

namespace klotski {

/// --------------------------------------- Group Type ----------------------------------------

uint32_t GroupType::group_num() const noexcept {
    return TYPE_ID_GROUP_NUM[type_id_];
}

uint32_t GroupType::group_num(const RawCode &raw_code) noexcept {
    return GroupType(raw_code).group_num();
}

uint32_t GroupType::group_num(const CommonCode &common_code) noexcept {
    return GroupType(common_code).group_num();
}

uint32_t GroupType::max_size() const noexcept {
    return TYPE_ID_GROUP_MAX_SIZE[type_id_];
}

uint32_t GroupType::max_size(const RawCode &raw_code) noexcept {
    return GroupType(raw_code).max_size();
}

uint32_t GroupType::max_size(const CommonCode &common_code) noexcept {
    return GroupType(common_code).max_size();
}

/// --------------------------------------- Group Case ----------------------------------------

GroupCase::info_t GroupCase::encode(const RawCode &raw_code) noexcept {
    return encode(raw_code.to_common_code());
}

CommonCode GroupCase::parse(const info_t &info) {
    auto cases = Group(info.type_id, info.group_id).cases();
    if (cases.size() <= info.group_index) {
        throw std::invalid_argument("group index overflow"); // check group index
    }

    std::vector<CommonCode> group(cases.begin(), cases.end());
    std::nth_element(group.begin(), group.begin() + info.group_index, group.end());
    return group[info.group_index]; // located nth as target
}

GroupCase::info_t GroupCase::encode(const CommonCode &common_code) noexcept {
    uint32_t group_index = 0;
    auto cases = Group::cases(common_code);
    std::vector<CommonCode> group(cases.begin(), cases.end());
    for (auto &&code: group) {
        if (code < common_code) {
            ++group_index; // locate group index
        }
    }

    GroupType type = GroupType(common_code);
    auto seed = *std::min_element(group.begin(), group.end());
    return {
        .type_id = static_cast<uint16_t>(type.unwrap()),
        .group_id = static_cast<uint16_t>(group_id(type, seed)),
        .group_index = group_index,
    };
}

/// ------------------------------------- Group Case Fast -------------------------------------

void GroupCase::demo() {
    std::cout << "ok" << std::endl;

    // short code -> group_id + group_index

    std::vector<uint32_t> group_info(SHORT_CODE_LIMIT);
    std::vector<std::vector<ShortCode>> group_data[TYPE_ID_LIMIT];

    ShortCode::speed_up(ShortCode::FAST);

    auto convert = [](const std::vector<RawCode> &raw_codes) -> std::vector<CommonCode> {
        return {raw_codes.begin(), raw_codes.end()};
    };

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto tid = GroupType(type_id);

        group_data[type_id].resize(tid.group_num());

        for (uint32_t group_id = 0; group_id < tid.group_num(); ++group_id) {
            auto group = Group(tid, group_id);

            auto cases = convert(group.cases());
            std::sort(cases.begin(), cases.end());

            for (uint32_t group_index = 0; group_index < cases.size(); ++group_index) {
                auto short_code = cases[group_index].to_short_code();
                group_info[short_code.unwrap()] = (group_id << 20) | group_index;
                group_data[type_id][group_id].emplace_back(short_code);
            }
        }
        std::cerr << type_id << std::endl;
    }


    std::cout << group_data[169][1][7472].to_common_code() << std::endl;
    std::cout << group_data[164][0][30833].to_common_code() << std::endl;

    auto src_1 = CommonCode(0x1A9BF0C00);
    auto tmp_1 = group_info[src_1.to_short_code().unwrap()];
    std::cout << GroupType(src_1).unwrap() << "-"
        << (tmp_1 >> 20) << "-" << (tmp_1 & 0xFFFFF) << std::endl;

    auto src_2 = CommonCode(0x4FEA13400);
    auto tmp_2 = group_info[src_2.to_short_code().unwrap()];
    std::cout << GroupType(src_2).unwrap() << "-"
        << (tmp_2 >> 20) << "-" << (tmp_2 & 0xFFFFF) << std::endl;

}

} // namespace klotski
