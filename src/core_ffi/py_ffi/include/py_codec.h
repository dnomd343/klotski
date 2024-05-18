#pragma once

#include "short_code/short_code.h"
#include "common_code/common_code.h"

namespace klotski::ffi {

using codec::ShortCode;
using codec::CommonCode;

class PyCommonCode;

// ------------------------------------------------------------------------------------- //

class PyShortCode {
public:
    explicit PyShortCode(uint32_t code);
    explicit PyShortCode(PyCommonCode code);
    explicit PyShortCode(std::string_view code);

	/// Get original value.
    [[nodiscard]] uint32_t value() const;

	/// Convert ShortCode to CommonCode.
    [[nodiscard]] PyCommonCode common_code() const;

	/// Verify ShortCode in u32 form.
    static bool check(uint32_t code);

	/// Verify ShortCode in string form.
    static bool check(std::string_view code);

	/// Build conversion index for ShortCode.
    static void speed_up(bool fast_mode);

	/// Wrapper of `__str__` method in Python.
    static std::string str(PyShortCode code);

	/// Wrapper of `__repr__` method in Python.
    static std::string repr(PyShortCode code);

private:
    ShortCode code_;
};

// ------------------------------------------------------------------------------------- //

class PyCommonCode {
public:
    explicit PyCommonCode(uint64_t code);
    explicit PyCommonCode(PyShortCode code);
    explicit PyCommonCode(std::string_view code);

	/// Get original value.
    [[nodiscard]] uint64_t value() const;

	/// Convert CommonCode to ShortCode.
    [[nodiscard]] PyShortCode short_code() const;

	/// Convert as string form.
	[[nodiscard]] std::string string(bool shorten) const;

	/// Verify CommonCode in u64 form.
    static bool check(uint64_t code);

	/// Verify CommonCode in string form.
    static bool check(std::string_view code);

	/// Wrapper of `__str__` method in Python.
	static std::string str(PyCommonCode code);

	/// Wrapper of `__repr__` method in Python.
	static std::string repr(PyCommonCode code);

private:
    CommonCode code_;
};

// ------------------------------------------------------------------------------------- //

} // namespace klotski::ffi
