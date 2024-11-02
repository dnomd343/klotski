#pragma once

namespace klotski::group {

inline codec::CommonCode GroupCases::obtain_code(CaseInfo info) {
    if (fast_) {
        return fast_obtain_code(info);
    }
    return tiny_obtain_code(info);
}

inline CaseInfo GroupCases::obtain_info(const codec::CommonCode common_code) {
    if (fast_) {
        return fast_obtain_info(common_code);
    }
    return tiny_obtain_info(common_code);
}

inline Group GroupCases::obtain_group(const codec::CommonCode common_code) {
    if (fast_) {
        return fast_obtain_group(common_code);
    }
    return Group::from_common_code(common_code);
}

inline Group GroupCases::obtain_group(const codec::ShortCode short_code) {
    if (fast_) {
        return fast_obtain_group(short_code);
    }
    return Group::from_short_code(short_code);
}

inline CaseInfo GroupCases::obtain_info(const codec::ShortCode short_code) {
    if (fast_) {
        return fast_obtain_info(short_code);
    }
    return tiny_obtain_info(short_code.to_common_code());
}

} // namespace klotski::group
