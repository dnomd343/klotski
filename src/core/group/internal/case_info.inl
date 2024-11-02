#pragma once

#include <format>

namespace klotski::group {

inline std::ostream& operator<<(std::ostream &out, CaseInfo self) {
    out << self.to_string();
    return out;
}

constexpr std::string CaseInfo::to_string() const {
    return std::format("{}-{}", group_.to_string(), case_id_);
}

constexpr Group CaseInfo::group() const {
    return group_;
}

constexpr uint32_t CaseInfo::case_id() const {
    return case_id_;
}

constexpr std::optional<CaseInfo> CaseInfo::create(Group group, uint32_t case_id) {
    if (case_id >= group.size()) {
        return std::nullopt;
    }
    return unsafe_create(group, case_id);
}

constexpr CaseInfo CaseInfo::unsafe_create(Group group, uint32_t case_id) {
    return {group, case_id};
}

constexpr CaseInfo::CaseInfo(Group group, uint32_t case_id) : group_(group), case_id_(case_id) {}

} // namespace klotski::group
