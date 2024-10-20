#include <pybind11/pybind11.h>

#include "include/py_cases.h"

namespace py = pybind11;

using klotski::ffi::PyCases;
using klotski::ffi::PyCasesIter;
using klotski::ffi::PyCommonCode;

using klotski::cases::RangesUnion;

size_t PyCases::size() const {
    size_t num = 0;
    for (const auto &x : ref()) {
        num += x.size();
    }
    return num;
}

//PyCases PyCases::from(RangesUnion &&data) {
//    return PyCases(std::move(data));
//}
//
//PyCases PyCases::from_ref(const RangesUnion &data) {
//    return PyCases(data);
//}
//
//const RangesUnion& PyCases::data_ref() const {
//    return data_;
//}

PyCommonCode PyCasesIter::next() {
    while (head_ < 16) {
        const auto &ranges = data_.ref()[head_];
        if (index_ < ranges.size()) {
            auto code = (head_ << 32) | ranges[index_++];
            return std::bit_cast<PyCommonCode>(code);
        }
        ++head_;
        index_ = 0;
    }
    throw py::stop_iteration();
}
