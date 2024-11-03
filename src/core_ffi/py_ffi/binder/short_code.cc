#include "binder.h"

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
