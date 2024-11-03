#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "py_ffi/cases.h"
#include "py_ffi/group.h"
#include "py_ffi/short_code.h"
#include "py_ffi/common_code.h"
#include "py_ffi/fast_cal.h"

using klotski::ffi::PyCases;
using klotski::ffi::PyCasesIter;
using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;
using klotski::ffi::PyGroupUnion;
using klotski::ffi::PyGroup;
using klotski::ffi::PyFastCal;

void bind_cases(const py::module_ &m);
void bind_short_code(const py::module_ &m);
void bind_common_code(const py::module_ &m);

void bind_group_union(const py::module_ &m);
void bind_group(const py::module_ &m);

void bind_fast_cal(const py::module_ &m);
