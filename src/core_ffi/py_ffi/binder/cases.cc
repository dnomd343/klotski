#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "include/py_cases.h"

namespace py = pybind11;

using klotski::ffi::PyCases;
using klotski::ffi::PyCasesIter;

void bind_cases(const py::module_ &m) {
    py::class_<PyCases>(m, "Cases")
        .def(py::self == py::self)
        .def("__len__", &PyCases::size)
        .def("__repr__", &PyCases::repr)
        .def("__getitem__", &PyCases::at)
        .def("__iter__", &PyCases::codes, py::keep_alive<0, 1>());

    py::class_<PyCasesIter>(m, "CasesIter")
        .def("__next__", &PyCasesIter::next);
}
