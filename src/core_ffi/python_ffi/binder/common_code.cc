#include "binder.h"

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

//        .def_property_readonly("str", &PyCommonCode::string)
        .def_property_readonly("value", &PyCommonCode::value)
        .def_property_readonly("short_code", &PyCommonCode::short_code)

        .def("next_cases", &PyCommonCode::next_cases)

        .def("to_string", &PyCommonCode::string, py::arg("shorten") = false)

        .def_static("check", static_cast<bool (*)(uint64_t)>(&PyCommonCode::check))
        .def_static("check", static_cast<bool (*)(std::string_view)>(&PyCommonCode::check));
}
