#include "binder.h"
#include "exception.h"

using klotski::ffi::PyExc_CodecError;
using klotski::ffi::PyExc_GroupError;

#include "group/group.h"

// static PyCases group_demo() {
//     auto group_union = klotski::group::GroupUnion::unsafe_create(169);
//     auto cases = PyCases::from(group_union.cases());
//     return cases;
// }

PYBIND11_MODULE(_klotski, m) {
    py::register_exception<PyExc_GroupError>(m, "GroupError", PyExc_ValueError);
    py::register_exception<PyExc_CodecError>(m, "CodecError", PyExc_ValueError);

    // m.def("group_demo", &group_demo);

    bind_klsk_cases(m);
    bind_klsk_code(m);

    bind_group(m);
    bind_group_union(m);
    bind_fast_cal(m);

    m.attr("__version__") = "version field";
}
