#include <pybind11/pybind11.h>

// #include <pybind11/stl.h>
// #include <pybind11/operators.h>

#include "py_exps.h"
#include "py_codec.h"

namespace py = pybind11;

using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;

using klotski::ffi::PyCodecExp;

PYBIND11_MODULE(klotski, m) {

    py::register_exception<PyCodecExp>(m, "CodecExp", PyExc_ValueError);

    py::class_<PyShortCode>(m, "ShortCode")
        .def(py::init<uint32_t>())
        .def(py::init<PyCommonCode>())
        .def(py::init<std::string_view>())

        .def("__str__", &PyShortCode::str)
        .def("__repr__", &PyShortCode::repr)

        .def_property_readonly("value", &PyShortCode::value)
        .def_property_readonly("common_code", &PyShortCode::common_code)

        .def_static("check", static_cast<bool (*)(uint32_t)>(&PyShortCode::check))
        .def_static("check", static_cast<bool (*)(std::string_view)>(&PyShortCode::check))
        .def_static("speed_up", &PyShortCode::speed_up, py::arg("fast_mode") = false);

    py::class_<PyCommonCode>(m, "CommonCode")
        .def(py::init<uint64_t>())
        .def(py::init<PyShortCode>())
        .def(py::init<std::string_view>())

        .def("__str__", &PyCommonCode::str)
        .def("__repr__", &PyCommonCode::repr)

        .def_property_readonly("value", &PyCommonCode::value)
        .def_property_readonly("short_code", &PyCommonCode::short_code)
        .def("string", &PyCommonCode::string, py::arg("shorten") = false)

        .def_static("check", static_cast<bool (*)(uint64_t)>(&PyCommonCode::check))
        .def_static("check", static_cast<bool (*)(std::string_view)>(&PyCommonCode::check));

    m.attr("__version__") = "version field";
}
