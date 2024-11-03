/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <vector>
#include <cstdint>
#include <group/group.h>

#include "py_ffi/cases.h"
#include "py_ffi/common_code.h"

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

private:
    Group group_;
};

class PyGroupUnion {
public:
    PyGroupUnion(uint8_t type_id);
    PyGroupUnion(PyShortCode short_code);
    PyGroupUnion(PyCommonCode common_code);

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
