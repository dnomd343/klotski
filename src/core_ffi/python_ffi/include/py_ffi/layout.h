/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <group/group.h>
#include <mover/mover.h> // TODO: move to `.cc` file
#include <common_code/common_code.h>

#include "py_ffi/group.h"
#include "py_ffi/short_code.h"

// TODO: add `copy` and `pickle` support

namespace klotski::ffi {

enum class PyBlock : uint8_t {
    SPACE = 0b000,
    B_1x2 = 0b001,
    B_2x1 = 0b010,
    B_1x1 = 0b011,
    B_2x2 = 0b100,
    FILL = 0b111,
};

enum class PyToward : uint8_t { // TODO: from `Group::Toward` directly
    A = 0,
    B = 1,
    C = 2,
    D = 3,
};

class PyLayout {
public:
    PyLayout() = delete;

    // ------------------------------------------------------------------------------------- //

    /// Construct from origin u64 value.
    explicit PyLayout(uint64_t code);

    /// Construct from origin string form.
    explicit PyLayout(std::string_view code);

    /// Construct from PyShortCode object.
    explicit PyLayout(PyShortCode code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Verify CommonCode in u64 form.
    static bool check(uint64_t code) noexcept;

    /// Verify CommonCode in string form.
    static bool check(std::string_view code) noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Get original value.
    [[nodiscard]] uint64_t value() const noexcept;

    /// Convert Layout as ShortCode.
    [[nodiscard]] PyShortCode short_code() const noexcept;

    /// Convert Layout as string form.
    [[nodiscard]] std::string string(bool shorten) const noexcept;

    // ------------------------------------------------------------------------------------- //

    /// Wrapper of `__str__` method in Python.
    static std::string str(PyLayout code) noexcept;

    /// Wrapper of `__repr__` method in Python.
    static std::string repr(PyLayout code) noexcept;

    // ------------------------------------------------------------------------------------- //

    [[nodiscard]] std::vector<PyLayout> next_cases() const noexcept;

    [[nodiscard]] std::array<PyBlock, 20> dump_seq() const noexcept;

    // ------------------------------------------------------------------------------------- //

    [[nodiscard]] uint8_t type_id() const noexcept {
        return group::GroupUnion::from_common_code(code_).unwrap();
    }

    [[nodiscard]] uint16_t pattern_id() const noexcept {
        return group_info().pattern_id();
    }

    [[nodiscard]] PyToward toward() const noexcept {
        const auto group = group::GroupCases::obtain_group(code_);
        return (PyToward)group.toward();
    }

    [[nodiscard]] PyGroup group_info() const noexcept {
        const auto group = group::GroupCases::obtain_group(code_);
        return std::bit_cast<PyGroup>(group);
    }

    [[nodiscard]] uint32_t case_id() const noexcept {
        const auto info = group::GroupCases::obtain_info(code_);
        return info.case_id();
    }

    [[nodiscard]] std::tuple<PyGroup, uint32_t> case_info() const noexcept {
        const auto info = group::GroupCases::obtain_info(code_);
        return std::make_tuple(std::bit_cast<PyGroup>(info.group()), info.case_id());
    }

    [[nodiscard]] uint8_t n_1x1() const noexcept {
        return std::get<2>(group::BLOCK_NUM[type_id()]);
    }

    [[nodiscard]] uint8_t n_1x2() const noexcept {
        return std::get<0>(group::BLOCK_NUM[type_id()]) - n_2x1();
    }

    [[nodiscard]] uint8_t n_2x1() const noexcept {
        return std::get<1>(group::BLOCK_NUM[type_id()]);
    }

    [[nodiscard]] bool is_vertical_mirror() const noexcept {
        return code_.is_vertical_mirror();
    }

    [[nodiscard]] bool is_horizontal_mirror() const noexcept {
        return code_.is_horizontal_mirror();
    }

    [[nodiscard]] PyLayout to_vertical_mirror() const noexcept {
        return std::bit_cast<PyLayout>(code_.to_vertical_mirror());
    }

    [[nodiscard]] PyLayout to_horizontal_mirror() const noexcept {
        return std::bit_cast<PyLayout>(code_.to_horizontal_mirror());
    }

    // ------------------------------------------------------------------------------------- //

private:
    codec::CommonCode code_;
};

static_assert(std::is_trivially_copyable_v<PyLayout>);
static_assert(sizeof(PyLayout) == sizeof(codec::CommonCode));

// ----------------------------------------------------------------------------------------- //

constexpr auto operator==(const PyLayout &lhs, const uint64_t rhs) {
    return lhs.value() == rhs;
}

constexpr auto operator<=>(const PyLayout &lhs, const uint64_t rhs) {
    return lhs.value() <=> rhs;
}

constexpr auto operator==(const PyLayout &lhs, const PyLayout &rhs) {
    return lhs.value() == rhs.value();
}

constexpr auto operator<=>(const PyLayout &lhs, const PyLayout &rhs) {
    return lhs.value() <=> rhs.value();
}

// ----------------------------------------------------------------------------------------- //

} // namespace klotski::ffi

template <>
struct std::hash<klotski::ffi::PyLayout> {
    size_t operator()(const klotski::ffi::PyLayout &layout) const noexcept {
        return std::hash<uint64_t>{}(layout.value());
    }
};
