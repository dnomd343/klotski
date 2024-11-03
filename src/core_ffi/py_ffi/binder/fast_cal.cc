#include <py_ffi/fast_cal.h>

#include "binder.h"

void bind_fast_cal(const py::module_ &m) {
    py::class_<PyFastCal>(m, "FastCal")
        .def(py::init<PyCommonCode>())
        .def("solve", &PyFastCal::solve)
        .def("exports", &PyFastCal::exports)
        .def("build_all", &PyFastCal::build_all)
        .def("backtrack", &PyFastCal::backtrack);
}
