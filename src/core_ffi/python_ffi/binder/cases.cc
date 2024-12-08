#include "binder.h"

void bind_cases(const py::module_ &mod) {
    py::class_<PyCases>(mod, "Cases")
        .def(py::self == py::self)
        .def("__len__", &PyCases::size)
        .def("__repr__", &PyCases::repr)
        .def("__getitem__", &PyCases::at)
        .def("__iter__", &PyCases::iter, py::keep_alive<0, 1>())
        .def_property_readonly_static("all_cases", [](const py::object&) {
            return PyCases::all_cases();
        });

    py::class_<PyCasesIter>(mod, "CasesIter")
        .def("__next__", &PyCasesIter::next);
}
