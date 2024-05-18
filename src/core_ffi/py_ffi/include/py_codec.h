#pragma once

#include "short_code/short_code.h"
#include "common_code/common_code.h"

namespace klotski::ffi {

using codec::ShortCode;
using codec::CommonCode;

class PyCommonCode;

// ----------------------------------------------------------------------------------------- //

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

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const PyShortCode &lhs, const uint32_t rhs) {
	return lhs.value() == rhs;
}

constexpr auto operator<=>(const PyShortCode &lhs, const uint32_t rhs) {
	return lhs.value() <=> rhs;
}

constexpr auto operator==(const PyShortCode &lhs, const PyShortCode &rhs) {
	return lhs.value() == rhs.value();
}

constexpr auto operator<=>(const PyShortCode &lhs, const PyShortCode &rhs) {
	return lhs.value() <=> rhs.value();
}

// ----------------------------------------------------------------------------------------- //

class PyCommonCode {
public:
    explicit PyCommonCode(uint64_t code);
    explicit PyCommonCode(PyShortCode code);
    explicit PyCommonCode(std::string_view code);

	/// Get original value.
    [[nodiscard]] uint64_t value() const;

	/// Convert as shorten string form.
	[[nodiscard]] std::string string() const;

	/// Convert CommonCode to ShortCode.
    [[nodiscard]] PyShortCode short_code() const;

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

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const PyCommonCode &lhs, const uint64_t rhs) {
	return lhs.value() == rhs;
}

constexpr auto operator<=>(const PyCommonCode &lhs, const uint64_t rhs) {
	return lhs.value() <=> rhs;
}

constexpr auto operator==(const PyCommonCode &lhs, const PyCommonCode &rhs) {
	return lhs.value() == rhs.value();
}

constexpr auto operator<=>(const PyCommonCode &lhs, const PyCommonCode &rhs) {
	return lhs.value() <=> rhs.value();
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::ffi

// ----------------------------------------------------------------------------------------- //

template<>
struct std::hash<klotski::ffi::PyShortCode> {
	size_t operator()(const klotski::ffi::PyShortCode &short_code) const noexcept {
		return std::hash<uint32_t>()(short_code.value());
	}
};

template<>
struct std::hash<klotski::ffi::PyCommonCode> {
	size_t operator()(const klotski::ffi::PyCommonCode &common_code) const noexcept {
		return std::hash<uint64_t>()(common_code.value());
	}
};

// ----------------------------------------------------------------------------------------- //
