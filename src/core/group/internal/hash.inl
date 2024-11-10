#pragma once

namespace std {

template <>
struct hash<klotski::group::Group> {
    constexpr std::size_t operator()(const klotski::group::Group &g) const noexcept {
        // TODO: perf hash alg
        return std::hash<uint64_t>{}(g.type_id() ^ g.pattern_id() ^ (int)g.toward());
    }
};

template <>
struct hash<klotski::group::GroupUnion> {
    constexpr std::size_t operator()(const klotski::group::GroupUnion &gu) const noexcept {
        return std::hash<uint32_t>{}(gu.unwrap());
    }
};

template <>
struct hash<klotski::group::CaseInfo> {
    constexpr std::size_t operator()(const klotski::group::CaseInfo &info) const noexcept {
        // TODO: perf hash alg
        const auto h1 = std::hash<klotski::group::Group>{}(info.group());
        const auto h2 = std::hash<uint32_t>{}(info.case_id());
        return h1 ^ h2;
    }
};

} // namespace std
