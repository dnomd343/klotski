#include <pybind11/operators.h>

#include "binder.h"
#include "py_ffi/short_code.h"
#include "py_ffi/common_code.h"

using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;

static void bind_common_code(const py::module_ &mod) {
    py::class_<PyCommonCode>(mod, "Code")
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
        .def("__str__", &PyCommonCode::str)
        .def("__int__", &PyCommonCode::value)
        .def("__repr__", &PyCommonCode::repr)

        .def("next_cases", &PyCommonCode::next_cases)
        // TODO: add fast_cal / fast_cal_multi / ...

        .def("to_short_code", &PyCommonCode::short_code)
        .def("to_string", &PyCommonCode::string, py::arg("shorten") = false)

        .def_property_readonly("value", &PyCommonCode::value)
        // TODO: add n_1x1 / n_1x2 / n_2x1 / ...

        .def_static("check", py::overload_cast<uint64_t>(&PyCommonCode::check))
        .def_static("check", py::overload_cast<std::string_view>(&PyCommonCode::check));
}

static void bind_short_code(const py::module_ &mod) {
    py::class_<PyShortCode>(mod, "ShortCode")
        .def(py::init<uint32_t>())
        .def(py::init<PyCommonCode>())
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

        .def("to_common_code", &PyShortCode::common_code)

        .def_static("check", py::overload_cast<uint32_t>(&PyShortCode::check))
        .def_static("check", py::overload_cast<std::string_view>(&PyShortCode::check))

        .def_static("speed_up", &PyShortCode::speed_up, py::arg("fast_mode") = false);
}

void bind_klsk_code(const py::module_ &mod) {
    bind_short_code(mod);
    bind_common_code(mod);
}
