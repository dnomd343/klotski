#pragma once

// namespace internal {
//
// class GroupImpl {
// public:
//     explicit GroupImpl(int flat_id) : flat_id_(flat_id) {}
//
//     const std::vector<codec::CommonCode>& cases();
//
//     int flat_id_;
//
//     static constexpr std::array<GroupImpl, 6> ins() {
//         return std::array<GroupImpl, 6> {};
//     }
// };
//
// } // namespace internal
//
// inline const std::vector<codec::CommonCode>& Group::cases() {
//     static auto kk = internal::GroupImpl::ins();
//     return kk[0].cases();
// }
