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