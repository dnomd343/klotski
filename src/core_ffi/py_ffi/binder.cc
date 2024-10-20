#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "py_exps.h"
#include "py_common_code.h"
#include "py_short_code.h"
#include "py_cases.h"
#include "py_group.h"

#include "py_exception.h"

namespace py = pybind11;

using klotski::ffi::PyCases;
//using klotski::ffi::PyCasesIter;

//using klotski::ffi::PyCodecExp;
using klotski::ffi::PyShortCode;
using klotski::ffi::PyCommonCode;

using klotski::ffi::PyExc_CodecError;

using klotski::ffi::PyGroup;
using klotski::ffi::PyGroupUnion;

extern void bind_cases(const py::module_ &m);

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

        .def("to_string", &PyCommonCode::string, py::arg("shorten") = false)

        .def_static("check", static_cast<bool (*)(uint64_t)>(&PyCommonCode::check))
        .def_static("check", static_cast<bool (*)(std::string_view)>(&PyCommonCode::check));
}

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

#include "group/group.h"
#include "all_cases/all_cases.h"

static PyCases group_demo() {
    auto group_union = klotski::cases::GroupUnion::unsafe_create(169);
    auto cases = PyCases::from(group_union.cases());
    return cases;
}

static PyCases all_cases() {
    return PyCases::from_ref(klotski::cases::AllCases::instance().fetch());
}

PYBIND11_MODULE(klotski, m) {
//    py::register_exception<PyCodecExp>(m, "CodecExp", PyExc_ValueError);
    py::register_exception<PyExc_CodecError>(m, "CodecError", PyExc_ValueError);

    m.def("all_cases", &all_cases);
    m.def("group_demo", &group_demo);

    bind_short_code(m);
    bind_common_code(m);

    bind_cases(m);

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

    py::class_<PyGroup>(m, "Group")
        .def_property_readonly("type_id", &PyGroup::type_id)
        .def_property_readonly("pattern_id", &PyGroup::pattern_id)

        .def("__str__", &PyGroup::to_string)

        .def("cases", &PyGroup::cases)
        .def("to_vertical_mirror", &PyGroup::to_vertical_mirror)
        .def("to_horizontal_mirror", &PyGroup::to_horizontal_mirror)

        .def_property_readonly("size", &PyGroup::size)
        .def_property_readonly("is_vertical_mirror", &PyGroup::is_vertical_mirror)
        .def_property_readonly("is_horizontal_mirror", &PyGroup::is_horizontal_mirror);

    m.attr("__version__") = "version field";
}
