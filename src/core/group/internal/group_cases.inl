#pragma once

namespace klotski::cases {

inline void GroupCases::build() {

}

inline void GroupCases::build_async(Executor &&executor, Notifier &&callback) {

}

inline codec::CommonCode GroupCases::parse(GroupCases::info_t info) {
    return codec::CommonCode::unsafe_create(0);
}

inline GroupCases::info_t GroupCases::group_info(codec::RawCode raw_code) {
    return info_t {};
}

inline GroupCases::info_t GroupCases::group_info(codec::ShortCode short_code) {
    return info_t {};
}

inline GroupCases::info_t GroupCases::group_info(codec::CommonCode common_code) {
    return info_t {};
}

} // namespace klotski::cases
