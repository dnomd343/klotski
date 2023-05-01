#include <algorithm>
#include <stdexcept>
#include "group.h"
#include "type_id.h"

namespace klotski {

std::mutex GroupCase::building_;
bool GroupCase::available_ = false;
std::vector<uint32_t> GroupCase::group_info_(SHORT_CODE_LIMIT);
std::vector<ShortCodes> GroupCase::group_data_[TYPE_ID_LIMIT];

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

CommonCode GroupCase::parse(const info_t &info) {
    if (available_) {
        return fast_decode(info);
    }
    return tiny_decode(info);
}

GroupCase::info_t GroupCase::encode(const RawCode &raw_code) noexcept {
    return encode(raw_code.to_common_code());
}

GroupCase::info_t GroupCase::encode(const CommonCode &common_code) noexcept {
    if (available_) {
        return fast_encode(common_code);
    }
    return tiny_encode(common_code);
}

/// ------------------------------------ Group Case Codec -------------------------------------

CommonCode GroupCase::tiny_decode(const info_t &info) {
    auto cases = Group(info.type_id, info.group_id).cases();
    if (cases.size() <= info.group_index) {
        throw std::invalid_argument("group index overflow"); // check group index
    }
    auto group = CommonCode::convert(cases);
    std::nth_element(group.begin(), group.begin() + info.group_index, group.end());
    return group[info.group_index]; // located nth as target
}

GroupCase::info_t GroupCase::tiny_encode(const CommonCode &common_code) noexcept {
    uint32_t group_index = 0;
    auto group = CommonCode::convert(Group::cases(common_code));
    for (auto &&tmp: group) {
        if (tmp < common_code) {
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

CommonCode GroupCase::fast_decode(const info_t &info) {
    if (info.type_id >= TYPE_ID_LIMIT) {
        throw std::invalid_argument("type id overflow");
    }
    if (info.group_id >= TYPE_ID_GROUP_NUM[info.type_id]) {
        throw std::invalid_argument("group id overflow");
    }
    auto &target_group = group_data_[info.type_id][info.group_id];
    if (info.group_index >= target_group.size()) {
        throw std::invalid_argument("group index overflow");
    }
    return target_group[info.group_index].to_common_code();
}

GroupCase::info_t GroupCase::fast_encode(const CommonCode &common_code) noexcept {
    auto info = group_info_[common_code.to_short_code().unwrap()];
    return {
        .type_id = static_cast<uint16_t>(GroupType(common_code).unwrap()),
        .group_id = static_cast<uint16_t>(info >> 20),
        .group_index = info & 0xFFFFF,
    };
}

/// ------------------------------------ Group Case Index -------------------------------------

void GroupCase::speed_up() {
    auto build_data = []() { // build group cases index
        ShortCode::speed_up(ShortCode::FAST);
        for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
            GroupCase::build_index(GroupType(type_id));
        }
    };
    if (!available_) {
        if (building_.try_lock()) { // mutex lock success
            build_data(); // start build process
            available_ = true;
        } else {
            building_.lock(); // blocking waiting
        }
        building_.unlock(); // release mutex
    }
}

void GroupCase::build_index(GroupType group_type) noexcept {
    group_data_[group_type.unwrap()].resize(group_type.group_num());
    for (uint32_t group_id = 0; group_id < group_type.group_num(); ++group_id) {
        auto cases = CommonCode::convert(Group(group_type, group_id).cases());
        std::sort(cases.begin(), cases.end());
        for (uint32_t group_index = 0; group_index < cases.size(); ++group_index) {
            auto short_code = cases[group_index].to_short_code();
            group_info_[short_code.unwrap()] = (group_id << 20) | group_index;
            group_data_[group_type.unwrap()][group_id].emplace_back(short_code);
        }
    }
}

} // namespace klotski
