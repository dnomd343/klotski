#pragma once

#include <bit>

#include "common_code/common_code.h"

namespace klotski::codec {

// ------------------------------------------------------------------------------------- //

inline ShortCode::ShortCode(const CommonCode common_code) {
    // TODO: test the affect of CPU branch prediction.
    if (cases::AllCases::instance().is_available()) {
        code_ = fast_encode(common_code.unwrap());
    } else {
        code_ = tiny_encode(common_code.unwrap());
    }
}

inline ShortCode ShortCode::unsafe_create(const uint32_t short_code) {
    return std::bit_cast<ShortCode>(short_code); // init directly
}

inline std::optional<ShortCode> ShortCode::create(const uint32_t short_code) {
    if (!check(short_code)) {
        return std::nullopt; // invalid short code
    }
    return unsafe_create(short_code);
}

// ------------------------------------------------------------------------------------- //

inline ShortCode::operator uint32_t() const {
    return code_;
}

inline bool ShortCode::check(const uint32_t short_code) {
    return short_code < SHORT_CODE_LIMIT; // [0, SHORT_CODE_LIMIT)
}

inline void ShortCode::speed_up(const bool fast_mode) {
    // TODO: keep one way change.
    if (fast_mode) {
        // cases::AllCases::instance().build();
        std::lock_guard guard {busy_};
        cases_ = &cases::AllCases::instance().fetch();
        stage_ = Stage::FAST;
    } else {
        std::lock_guard guard {busy_};

        // TODO: skip if stage_ is FAST

        ranges_ = &cases::BasicRanges::instance().fetch();
        stage_ = Stage::TINY;
    }
}

#ifndef KLSK_NDEBUG
inline std::ostream& operator<<(std::ostream &out, const ShortCode self) {
    out << ShortCode::string_encode(self.code_);
    return out;
}
#endif

// ----------------------------------------------------------------------------------------- //

inline uint32_t ShortCode::unwrap() const {
    return code_;
}

inline std::string ShortCode::to_string() const {
    return string_encode(code_);
}

inline CommonCode ShortCode::to_common_code() const {
    // TODO: test the affect of CPU branch prediction.
    // if (cases::AllCases::instance().is_available()) {
    //     return CommonCode::unsafe_create(fast_decode(code_));
    // }
    // return CommonCode::unsafe_create(tiny_decode(code_));

    switch (stage_) {
    case Stage::UNINIT:
        // TODO: do speed up
        // speed_up(false); // FIXME: slow about 3%
    case Stage::TINY:
        return CommonCode::unsafe_create(tiny_decode(code_));
    case Stage::FAST:
        return CommonCode::unsafe_create(fast_decode(code_));
    }
}

// ----------------------------------------------------------------------------------------- //

inline std::optional<ShortCode> ShortCode::from_string(const std::string_view short_code) {
    return string_decode(short_code).transform(unsafe_create);
}

// ----------------------------------------------------------------------------------------- //

inline ShortCode ShortCode::from_common_code(const CommonCode common_code) {
    return common_code.to_short_code();
}

inline std::optional<ShortCode> ShortCode::from_common_code(const uint64_t common_code) {
    const auto convert = [](const CommonCode code) {
        return code.to_short_code();
    };
    return CommonCode::create(common_code).transform(convert);
}

inline std::optional<ShortCode> ShortCode::from_common_code(const std::string_view common_code) {
    const auto convert = [](const CommonCode code) {
        return code.to_short_code();
    };
    return CommonCode::from_string(common_code).transform(convert);
}

// ------------------------------------------------------------------------------------- //

constexpr auto operator==(const ShortCode &lhs, const uint32_t rhs) {
    return lhs.code_ == rhs;
}

constexpr auto operator<=>(const ShortCode &lhs, const uint32_t rhs) {
    return lhs.code_ <=> rhs;
}

constexpr auto operator==(const ShortCode &lhs, const ShortCode &rhs) {
    return lhs.code_ == rhs.code_;
}

constexpr auto operator<=>(const ShortCode &lhs, const ShortCode &rhs) {
    return lhs.code_ <=> rhs.code_;
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::codec
