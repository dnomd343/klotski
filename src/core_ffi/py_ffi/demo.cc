#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <common_code/common_code.h>

namespace py = pybind11;

// class CommonCode {
// public:
//     explicit CommonCode(uint64_t val) : value_(val) {}
//
//     uint64_t Value() const {
//         return value_;
//     }
//
//     static std::optional<CommonCode> Create(uint64_t val) {
//         if (val == 343) {
//             return CommonCode {val};
//         }
//         return std::nullopt;
//     }
//
// private:
//     uint64_t value_ {343};
// };

using klotski::codec::CommonCode;

PYBIND11_MODULE(klotski, m) {

    py::class_<CommonCode>(m, "CommonCode")
        // .def(py::init<uint64_t>())
        .def("to_string", &CommonCode::to_string, py::arg("shorten") = false)
        .def_property_readonly("value", &CommonCode::unwrap)
        // .def_static("from_val", &CommonCode::Create);
        .def_static("create", &CommonCode::create);

    m.attr("__version__") = "version field";
}
