#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "py_exps.h"
#include "py_codec.h"

namespace py = pybind11;

using klotski::ffi::PyCodecExp;
using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;

void bind_common_code(const py::module_ &m) {
    py::class_<PyCommonCode>(m, "CommonCode")
        .def(py::init<uint64_t>())
        .def(py::init<PyShortCode>())
        .def(py::init<std::string_view>())

        .def(py::hash(py::self))
        .def("__str__", &PyCommonCode::str)
        .def("__int__", &PyCommonCode::value)
        .def("__repr__", &PyCommonCode::repr)

        .def(py::self == py::self)
        .def(py::self < py::self).def(py::self <= py::self)
        .def(py::self > py::self).def(py::self >= py::self)

        .def(py::self == uint64_t())
        .def(py::self < uint64_t()).def(py::self <= uint64_t())
        .def(py::self > uint64_t()).def(py::self >= uint64_t())

        .def_property_readonly("str", &PyCommonCode::string)
        .def_property_readonly("value", &PyCommonCode::value)
        .def_property_readonly("short_code", &PyCommonCode::short_code)

        .def_static("check", static_cast<bool (*)(uint64_t)>(&PyCommonCode::check))
        .def_static("check", static_cast<bool (*)(std::string_view)>(&PyCommonCode::check));
}

void bind_short_code(const py::module_ &m) {
    py::class_<PyShortCode>(m, "ShortCode")
        .def(py::init<uint32_t>())
        .def(py::init<PyCommonCode>())
        .def(py::init<std::string_view>())

        .def(py::hash(py::self))
        .def("__str__", &PyShortCode::str)
        .def("__int__", &PyShortCode::value)
        .def("__repr__", &PyShortCode::repr)

        .def(py::self == py::self)
        .def(py::self < py::self).def(py::self <= py::self)
        .def(py::self > py::self).def(py::self >= py::self)

        .def(py::self == uint32_t())
        .def(py::self < uint32_t()).def(py::self <= uint32_t())
        .def(py::self > uint32_t()).def(py::self >= uint32_t())

        .def_property_readonly("value", &PyShortCode::value)
        .def_property_readonly("common_code", &PyShortCode::common_code)

        .def_static("check", static_cast<bool (*)(uint32_t)>(&PyShortCode::check))
        .def_static("check", static_cast<bool (*)(std::string_view)>(&PyShortCode::check))
        .def_static("speed_up", &PyShortCode::speed_up, py::arg("fast_mode") = false);
}

PYBIND11_MODULE(klotski, m) {
    py::register_exception<PyCodecExp>(m, "CodecExp", PyExc_ValueError);

    bind_short_code(m);
    bind_common_code(m);

    m.attr("__version__") = "version field";
}
