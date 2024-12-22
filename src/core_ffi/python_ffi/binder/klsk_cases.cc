#include <pybind11/operators.h>

#include "binder.h"
#include "py_ffi/cases.h"
#include "py_ffi/layout.h"

using klotski::ffi::PyCases;
using klotski::ffi::PyCasesIter;

static void bind_cases(const py::module_ &mod) {
    py::class_<PyCases>(mod, "Cases")
        .def(py::self == py::self)
        .def("__len__", &PyCases::size)
        .def("__repr__", &PyCases::repr)
        .def("__getitem__", &PyCases::at)
        .def("__iter__", &PyCases::iter, py::keep_alive<0, 1>())
        .def_property_readonly_static("all_cases", [](const py::object&) {
            return PyCases::all_cases();
        });
}

static void bind_cases_iter(const py::module_ &mod) {
    py::class_<PyCasesIter>(mod, "CasesIter")
        .def("__next__", &PyCasesIter::next);
}

void bind_klsk_cases(const py::module_ &mod) {
    bind_cases(mod);
    bind_cases_iter(mod);
}
