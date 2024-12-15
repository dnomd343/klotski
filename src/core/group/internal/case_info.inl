#pragma once

#include <format>

namespace klotski::group {

// ----------------------------------------------------------------------------------------- //

constexpr Group CaseInfo::group() const {
    return group_;
}

constexpr uint32_t CaseInfo::case_id() const {
    return case_id_;
}

// ----------------------------------------------------------------------------------------- //

KLSK_INLINE_H std::string CaseInfo::to_string() const {
    // TODO: benchmark of format `{}-{}{}-{}`
    return std::format("{}-{}", group_.to_string(), case_id_);
}

#ifndef KLSK_NDEBUG
inline std::ostream& operator<<(std::ostream &out, const CaseInfo self) {
    out << self.to_string();
    return out;
}
#endif

constexpr auto operator==(const CaseInfo &lhs, const CaseInfo &rhs) {
    return lhs.group_ == rhs.group_ && lhs.case_id_ == rhs.case_id_;
}

// ----------------------------------------------------------------------------------------- //

constexpr CaseInfo::CaseInfo(const Group group, const uint32_t case_id)
    : group_(group), case_id_(case_id) {}

constexpr CaseInfo CaseInfo::unsafe_create(const Group group, const uint32_t case_id) {
    return {group, case_id};
}

constexpr std::optional<CaseInfo> CaseInfo::create(const Group group, const uint32_t case_id) {
    if (case_id >= group.size()) {
        return std::nullopt;
    }
    return unsafe_create(group, case_id);
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::group
