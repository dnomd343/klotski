#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "binder.h"
#include "py_ffi/layout.h"
#include "py_ffi/short_code.h"

using klotski::ffi::PyLayout;
using klotski::ffi::PyShortCode;

static void bind_layout(const py::module_ &mod) {
    py::class_<PyLayout>(mod, "Layout")
        .def(py::init<uint64_t>())
        .def(py::init<PyShortCode>())
        .def(py::init<std::string_view>())

        .def(py::self == py::self)
        .def(py::self < py::self).def(py::self <= py::self)
        .def(py::self > py::self).def(py::self >= py::self)
        .def(py::self == uint64_t())
        .def(py::self < uint64_t()).def(py::self <= uint64_t())
        .def(py::self > uint64_t()).def(py::self >= uint64_t())

        .def(py::hash(py::self))
        .def("__str__", &PyLayout::str)
        .def("__int__", &PyLayout::value)
        .def("__repr__", &PyLayout::repr)

        .def("next_cases", &PyLayout::next_cases)
        // TODO: add fast_cal / fast_cal_multi / ...

        .def("to_short_code", &PyLayout::short_code)
        .def("to_string", &PyLayout::string, py::arg("shorten") = false)

        .def_property_readonly("value", &PyLayout::value)
        // TODO: add n_1x1 / n_1x2 / n_2x1 / ...

        .def_static("check", py::overload_cast<uint64_t>(&PyLayout::check))
        .def_static("check", py::overload_cast<std::string_view>(&PyLayout::check));
}

static void bind_short_code(const py::module_ &mod) {
    py::class_<PyShortCode>(mod, "ShortCode")
        .def(py::init<uint32_t>())
        .def(py::init<PyLayout>())
        .def(py::init<std::string_view>())

        .def(py::self == py::self)
        .def(py::self < py::self).def(py::self <= py::self)
        .def(py::self > py::self).def(py::self >= py::self)
        .def(py::self == uint32_t())
        .def(py::self < uint32_t()).def(py::self <= uint32_t())
        .def(py::self > uint32_t()).def(py::self >= uint32_t())

        .def(py::hash(py::self))
        .def("__str__", &PyShortCode::str)
        .def("__int__", &PyShortCode::value)
        .def("__repr__", &PyShortCode::repr)

        .def_property_readonly("value", &PyShortCode::value)

        .def("to_layout", &PyShortCode::layout)

        .def_static("check", py::overload_cast<uint32_t>(&PyShortCode::check))
        .def_static("check", py::overload_cast<std::string_view>(&PyShortCode::check))

        .def_static("speed_up", &PyShortCode::speed_up, py::arg("fast_mode") = false);
}

void bind_klsk_code(const py::module_ &mod) {
    bind_layout(mod);
    bind_short_code(mod);
}
