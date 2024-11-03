#include "binder.h"

void bind_group_union(const py::module_ &m) {
    py::class_<PyGroupUnion>(m, "GroupUnion")
        .def(py::init<uint8_t>())
        .def(py::init<PyShortCode>())
        .def(py::init<PyCommonCode>())

//        .def(py::hash(py::self))
//        .def(py::self == py::self)
//        .def("__str__", &PyShortCode::str)
        .def("__int__", &PyGroupUnion::value)
//        .def("__repr__", &PyGroupUnion::repr)

        .def("cases", &PyGroupUnion::cases)
        .def("groups", &PyGroupUnion::groups)

        .def_property_readonly("size", &PyGroupUnion::size)
        .def_property_readonly("value", &PyGroupUnion::value)
        .def_property_readonly("group_num", &PyGroupUnion::group_num)
        .def_property_readonly("pattern_num", &PyGroupUnion::pattern_num)
        .def_property_readonly("max_group_size", &PyGroupUnion::max_group_size);
}
