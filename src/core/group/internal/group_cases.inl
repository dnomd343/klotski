#pragma once

#include <format>

namespace klotski::cases {

inline std::ostream& operator<<(std::ostream &out, GroupCases::CaseInfo self) {
    out << self.to_string();
    return out;
}

inline std::string GroupCases::CaseInfo::to_string() const {
    return std::format("{}-{}", group_.to_string(), case_id_);
}

constexpr Group GroupCases::CaseInfo::group() const {
    return group_;
}

constexpr uint32_t GroupCases::CaseInfo::case_id() const {
    return case_id_;
}

inline std::optional<GroupCases::CaseInfo> GroupCases::CaseInfo::create(klotski::cases::Group group, uint32_t case_id) {
    if (case_id >= group.size()) {
        return std::nullopt;
    }
    return unsafe_create(group, case_id);
}

inline GroupCases::CaseInfo GroupCases::CaseInfo::unsafe_create(klotski::cases::Group group, uint32_t case_id) {
    return {group, case_id};
}

inline codec::CommonCode GroupCases::obtain_code(CaseInfo info) {
    if (fast_) {
        return fast_obtain_code(info);
    }
    return tiny_obtain_code(info);
}

inline GroupCases::CaseInfo GroupCases::obtain_info(codec::CommonCode common_code) {
    if (fast_) {
        return fast_obtain_info(common_code);
    }
    return tiny_obtain_info(common_code);
}

inline Group GroupCases::obtain_group(codec::CommonCode common_code) {
    if (fast_) {
        return fast_obtain_group(common_code);
    }
    return Group::from_common_code(common_code);
}

inline Group GroupCases::obtain_group(codec::ShortCode short_code) {
    if (fast_) {
        return fast_obtain_group(short_code);
    }
    return Group::from_short_code(short_code);
}

inline GroupCases::CaseInfo GroupCases::obtain_info(codec::ShortCode short_code) {
    if (fast_) {
        return fast_obtain_info(short_code);
    }
    return tiny_obtain_info(short_code.to_common_code());
}

} // namespace klotski::cases
