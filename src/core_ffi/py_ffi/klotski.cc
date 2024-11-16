#include <pybind11/pybind11.h>
// #include <pybind11/stl.h>

// #include "py_ffi/common_code.h"
// #include "py_ffi/short_code.h"
// #include "py_ffi/cases.h"
// #include "py_ffi/group.h"

#include "exception.h"

#include "binder.h"

namespace py = pybind11;

using klotski::ffi::PyExc_CodecError;
using klotski::ffi::PyExc_GroupError;

#include "group/group.h"
#include "all_cases/all_cases.h"

// static PyCases group_demo() {
//     auto group_union = klotski::group::GroupUnion::unsafe_create(169);
//     auto cases = PyCases::from(group_union.cases());
//     return cases;
// }
//
// static PyCases all_cases() {
//     return PyCases::from_ref(klotski::cases::AllCases::instance().fetch());
// }

PYBIND11_MODULE(_klotski, m) {
    py::register_exception<PyExc_GroupError>(m, "GroupError", PyExc_ValueError);
    py::register_exception<PyExc_CodecError>(m, "CodecError", PyExc_ValueError);

    // m.def("all_cases", &all_cases);
    // m.def("group_demo", &group_demo);

    bind_cases(m);
    bind_short_code(m);
    bind_common_code(m);
    bind_group(m);
    bind_group_union(m);
    bind_fast_cal(m);

    m.attr("__version__") = "version field";
}
