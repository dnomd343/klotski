#include <pybind11/stl.h>
#include <py_ffi/fast_cal.h>

#include "binder.h"

#include "py_ffi/cases.h"
#include "py_ffi/group.h"
#include "py_ffi/short_code.h"
#include "py_ffi/layout.h"
#include "py_ffi/fast_cal.h"

using klotski::ffi::PyCases;
using klotski::ffi::PyCasesIter;
using klotski::ffi::PyShortCode;
using klotski::ffi::PyLayout;
using klotski::ffi::PyGroupUnion;
using klotski::ffi::PyGroup;
using klotski::ffi::PyFastCal;

void bind_fast_cal(const py::module_ &m) {
    py::class_<PyFastCal>(m, "FastCal")
        .def(py::init<PyLayout>())
        .def("solve", &PyFastCal::solve)
        .def("exports", &PyFastCal::exports)
        .def("build_all", &PyFastCal::build_all)
        .def("backtrack", &PyFastCal::backtrack);
}
