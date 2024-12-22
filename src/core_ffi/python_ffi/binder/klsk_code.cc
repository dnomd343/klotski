#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <py_ffi/speed_up.h>

#include "binder.h"
#include "py_ffi/layout.h"
#include "py_ffi/short_code.h"

using klotski::ffi::PyBlock;
using klotski::ffi::PyLayout;
using klotski::ffi::PyToward;
using klotski::ffi::PySpeedUp;
using klotski::ffi::PyShortCode;

static void bind_speed_up(const py::module_ &mod) {
    py::class_<PySpeedUp>(mod, "SpeedUp")
        .def_static("build_stage_0", &PySpeedUp::stage_0)
        .def_static("build_stage_1", &PySpeedUp::stage_1)
        .def_static("build_stage_2", &PySpeedUp::stage_2)
        .def_property_readonly_static("is_stage_0", [](const py::object&) { return PySpeedUp::is_stage_0(); })
        .def_property_readonly_static("is_stage_1", [](const py::object&) { return PySpeedUp::is_stage_1(); })
        .def_property_readonly_static("is_stage_2", [](const py::object&) { return PySpeedUp::is_stage_2(); });
}

static void bind_layout(const py::module_ &mod) {
    py::enum_<PyBlock>(mod, "Block")
        .value("SPACE", PyBlock::SPACE)
        .value("B_1x1", PyBlock::B_1x1)
        .value("B_1x2", PyBlock::B_1x2)
        .value("B_2x1", PyBlock::B_2x1)
        .value("B_2x2", PyBlock::B_2x2)
        .value("FILL", PyBlock::FILL);

    py::enum_<PyToward>(mod, "Toward")
        .value("A", PyToward::A)
        .value("B", PyToward::B)
        .value("C", PyToward::C)
        .value("D", PyToward::D);

    py::class_<PyLayout>(mod, "Layout")
        .def(py::init<uint64_t>())
        .def(py::init<PyShortCode>())
        .def(py::init<std::string_view>())

        .def(py::self == py::self)
        .def(py::self < py::self).def(py::self <= py::self)
        .def(py::self > py::self).def(py::self >= py::self)
        .def(py::self == uint64_t())
        .def(py::self < uint64_t()).def(py::self <= uint64_t())
        .def(py::self > uint64_t()).def(py::self >= uint64_t())

        .def(py::hash(py::self))
        .def("__str__", &PyLayout::str)
        .def("__int__", &PyLayout::value)
        .def("__repr__", &PyLayout::repr)

        .def("next_cases", &PyLayout::next_cases)
        .def("dump_seq", &PyLayout::dump_seq)
        // TODO: add fast_cal / fast_cal_multi / ...

        .def("to_short_code", &PyLayout::short_code)
        .def("to_string", &PyLayout::string, py::arg("shorten") = false)

        .def_property_readonly("value", &PyLayout::value)
        .def_property_readonly("n_1x1", &PyLayout::n_1x1)
        .def_property_readonly("n_1x2", &PyLayout::n_1x2)
        .def_property_readonly("n_2x1", &PyLayout::n_2x1)
        .def_property_readonly("n_2x2", [](const PyLayout&) { return 1; })

        .def_property_readonly("type_id", &PyLayout::type_id)
        .def_property_readonly("pattern_id", &PyLayout::pattern_id)
        .def_property_readonly("case_id", &PyLayout::case_id)
        .def_property_readonly("toward", &PyLayout::toward)
        .def_property_readonly("group", &PyLayout::group_info)
        .def_property_readonly("case_info", &PyLayout::case_info)

        .def("to_vertical_mirror", &PyLayout::to_vertical_mirror)
        .def("to_horizontal_mirror", &PyLayout::to_horizontal_mirror)
        .def_property_readonly("is_vertical_mirror", &PyLayout::is_vertical_mirror)
        .def_property_readonly("is_horizontal_mirror", &PyLayout::is_horizontal_mirror)

        .def_static("check", py::overload_cast<uint64_t>(&PyLayout::check))
        .def_static("check", py::overload_cast<std::string_view>(&PyLayout::check));
}

static void bind_short_code(const py::module_ &mod) {
    py::class_<PyShortCode>(mod, "ShortCode")
        .def(py::init<uint32_t>())
        .def(py::init<PyLayout>())
        .def(py::init<std::string_view>())

        .def(py::self == py::self)
        .def(py::self < py::self).def(py::self <= py::self)
        .def(py::self > py::self).def(py::self >= py::self)
        .def(py::self == uint32_t())
        .def(py::self < uint32_t()).def(py::self <= uint32_t())
        .def(py::self > uint32_t()).def(py::self >= uint32_t())

        .def(py::hash(py::self))
        .def("__str__", &PyShortCode::str)
        .def("__int__", &PyShortCode::value)
        .def("__repr__", &PyShortCode::repr)

        .def_property_readonly("value", &PyShortCode::value)

        .def("to_layout", &PyShortCode::layout)

        .def_static("check", py::overload_cast<uint32_t>(&PyShortCode::check))
        .def_static("check", py::overload_cast<std::string_view>(&PyShortCode::check));
}

void bind_klsk_code(const py::module_ &mod) {
    bind_layout(mod);
    bind_short_code(mod);
    bind_speed_up(mod);
}
