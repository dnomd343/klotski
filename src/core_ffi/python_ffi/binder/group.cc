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

void bind_group(const py::module_ &m) {
    py::class_<PyGroup>(m, "Group")
        .def_property_readonly("type_id", &PyGroup::type_id)
        .def_property_readonly("pattern_id", &PyGroup::pattern_id)

        .def("__str__", &PyGroup::to_string)
        .def("__repr__", &PyGroup::repr)

        .def("cases", &PyGroup::cases)
        .def("to_vertical_mirror", &PyGroup::to_vertical_mirror)
        .def("to_horizontal_mirror", &PyGroup::to_horizontal_mirror)

        .def_property_readonly("size", &PyGroup::size)
        .def_property_readonly("is_vertical_mirror", &PyGroup::is_vertical_mirror)
        .def_property_readonly("is_horizontal_mirror", &PyGroup::is_horizontal_mirror);
}
