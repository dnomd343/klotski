#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_klsk_code(const py::module_ &mod);

void bind_klsk_cases(const py::module_ &mod);

// TODO: add `bind_klsk_group` and `bind_klsk_fast_cal`

void bind_group_union(const py::module_ &m);
void bind_group(const py::module_ &m);
void bind_fast_cal(const py::module_ &m);
