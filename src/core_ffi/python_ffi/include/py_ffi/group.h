/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <vector>
#include <cstdint>
#include <group/group.h>

#include "py_ffi/cases.h"
#include "py_ffi/short_code.h"

namespace klotski::ffi {

using group::Group;
using group::GroupUnion;

class PyGroup {
public:
//    [[nodiscard]] Toward toward() const;

    [[nodiscard]] uint32_t type_id() const;

    [[nodiscard]] uint32_t pattern_id() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] PyCases cases() const;

    [[nodiscard]] uint32_t size() const;

//    [[nodiscard]] MirrorType mirror_type() const;

    [[nodiscard]] bool is_vertical_mirror() const;

    [[nodiscard]] bool is_horizontal_mirror() const;

    [[nodiscard]] PyGroup to_vertical_mirror() const;

    [[nodiscard]] PyGroup to_horizontal_mirror() const;

    static std::string repr(const PyGroup group) noexcept {
        return std::format("<klotski.Group '{}'>", group.to_string()); // TODO: maybe add size info
    }

private:
    Group group_;
};

class PyGroupUnion {
public:
    explicit PyGroupUnion(uint8_t type_id);
    explicit PyGroupUnion(PyLayout layout);
    explicit PyGroupUnion(PyShortCode short_code);

    [[nodiscard]] uint32_t value() const;

    [[nodiscard]] uint32_t size() const;

    [[nodiscard]] uint32_t group_num() const;

    [[nodiscard]] uint32_t pattern_num() const;

    [[nodiscard]] uint32_t max_group_size() const;

    [[nodiscard]] PyCases cases() const;

    [[nodiscard]] std::vector<PyGroup> groups() const;

private:
    GroupUnion group_union_;
};

} // namespace klotski::ffi
