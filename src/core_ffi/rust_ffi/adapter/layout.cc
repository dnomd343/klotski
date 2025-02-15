#include "rust_ffi/include/layout.h"

#include <group/group.h>
#include <mover/mover.h>
#include <common_code/common_code.h>

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::mover::MaskMover;

using klotski::group::Group;
using klotski::group::BLOCK_NUM;
using klotski::group::GroupCases;
using klotski::group::GroupUnion;

using klotski::ffi::RsLayout;
using klotski::ffi::RsShortCode;

// TODO: it seems that cxx.rs not support `std::optional`
uint64_t klotski::ffi::layout_from_str(const rust::Str s) {
    const std::string_view sv {s.data(), s.length()};
    if (const auto ret = CommonCode::from_string(sv); ret.has_value()) {
        return ret.value().unwrap();
    }
    return 0x10FFFFFFFF; // return invalid value for now
}

bool klotski::ffi::layout_check(const uint64_t val) {
    return CommonCode::check(val);
}

rust::String klotski::ffi::layout_to_str(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.to_string();
}

rust::String klotski::ffi::layout_to_shorten_str(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.to_shorten_string();
}

uint32_t klotski::ffi::layout_to_short_code(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.to_short_code().code;
}

bool klotski::ffi::layout_is_horizontal_mirror(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.is_horizontal_mirror();
}

bool klotski::ffi::layout_is_vertical_mirror(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.is_vertical_mirror();
}

uint64_t klotski::ffi::layout_to_horizontal_mirror(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.to_horizontal_mirror().code;
}

uint64_t klotski::ffi::layout_to_vertical_mirror(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.to_vertical_mirror().code;
}

uint8_t klotski::ffi::layout_n_1x1(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.n_1x1();
}

uint8_t klotski::ffi::layout_n_1x2(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.n_1x2();
}

uint8_t klotski::ffi::layout_n_2x1(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.n_2x1();
}

uint8_t klotski::ffi::layout_type_id(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.type_id();
}

uint16_t klotski::ffi::layout_pattern_id(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.pattern_id();
}

uint8_t klotski::ffi::layout_toward_char(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.toward_char();
}

uint32_t klotski::ffi::layout_case_id(uint64_t val) {
    const auto layout = RsLayout {val};
    return layout.case_id();
}

rust::Vec<uint64_t> klotski::ffi::layout_next_cases(uint64_t val) {
    std::vector<CommonCode> result;
    auto mover = MaskMover([&result](const RawCode code, uint64_t) {
        result.emplace_back(code.to_common_code());
    });
    mover.next_cases(CommonCode::unsafe_create(val).to_raw_code(), 0);

    rust::Vec<uint64_t> vec;
    for (auto x : result) {
        vec.emplace_back(x.unwrap());
    }
    return vec;
}

rust::String RsLayout::to_string() const noexcept {
    return CommonCode::unsafe_create(code).to_string();
}

rust::String RsLayout::to_shorten_string() const noexcept {
    return CommonCode::unsafe_create(code).to_string(true);
}

RsShortCode RsLayout::to_short_code() const noexcept {
    return {CommonCode::unsafe_create(code).to_short_code().unwrap()};
}

bool RsLayout::is_vertical_mirror() const noexcept {
    return CommonCode::unsafe_create(code).is_vertical_mirror();
}

bool RsLayout::is_horizontal_mirror() const noexcept {
    return CommonCode::unsafe_create(code).is_horizontal_mirror();
}

RsLayout RsLayout::to_vertical_mirror() const noexcept {
    return {CommonCode::unsafe_create(code).to_vertical_mirror().unwrap()};
}

RsLayout RsLayout::to_horizontal_mirror() const noexcept {
    return {CommonCode::unsafe_create(code).to_horizontal_mirror().unwrap()};
}

uint8_t RsLayout::n_1x1() const noexcept {
    return std::get<2>(BLOCK_NUM[type_id()]);
}

uint8_t RsLayout::n_1x2() const noexcept {
    return std::get<0>(BLOCK_NUM[type_id()]) - n_2x1();
}

uint8_t RsLayout::n_2x1() const noexcept {
    return std::get<1>(BLOCK_NUM[type_id()]);
}

uint8_t RsLayout::n_2x2() const noexcept {
    return 1;
}

uint8_t RsLayout::type_id() const noexcept {
    return GroupUnion::from_common_code(CommonCode::unsafe_create(code)).unwrap();
}

uint16_t RsLayout::pattern_id() const noexcept {
    return GroupCases::obtain_group(CommonCode::unsafe_create(code)).pattern_id();
}

uint8_t RsLayout::toward_char() const noexcept {
    return GroupCases::obtain_group(CommonCode::unsafe_create(code)).toward_char();
}

uint32_t RsLayout::case_id() const noexcept {
    return GroupCases::obtain_info(CommonCode::unsafe_create(code)).case_id();
}

rust::Vec<RsLayout> RsLayout::next_cases() const noexcept {
    std::vector<CommonCode> result;
    auto mover = MaskMover([&result](const RawCode code, uint64_t) {
        result.emplace_back(code.to_common_code());
    });
    mover.next_cases(CommonCode::unsafe_create(code).to_raw_code(), 0);

    rust::Vec<RsLayout> vec;
    for (auto x : result) {
        vec.emplace_back(RsLayout(x.unwrap()));
    }
    return vec;
}
