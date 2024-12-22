#include <format>
#include <pybind11/pybind11.h>

#include "py_ffi/cases.h"
#include "py_ffi/layout.h"

namespace py = pybind11;

using namespace klotski::ffi;
using klotski::cases::AllCases;

// ----------------------------------------------------------------------------------------- //

PyCasesIter::PyCasesIter(const RangesUnion &data) noexcept : data_(data) {}

PyLayout PyCasesIter::next() {
    while (head_ < 16) {
        if (const auto &ranges = data_.ranges(head_); index_ < ranges.size()) {
            return std::bit_cast<PyLayout>((head_ << 32) | ranges[index_++]);
        }
        index_ = 0, ++head_;
    }
    throw py::stop_iteration();
}

// ----------------------------------------------------------------------------------------- //

size_t PyCases::size() const noexcept {
    return data_ref().size();
}

PyCasesIter PyCases::iter() const noexcept {
    return PyCasesIter(data_ref());
}

PyLayout PyCases::at(const int32_t index) const {
    const auto size_ = static_cast<int32_t>(size());
    if (index >= size_ || index < -size_) {
        throw py::index_error("cases index out of range");
    }
    const auto code = data_ref()[index < 0 ? index + size_ : index];
    return std::bit_cast<PyLayout>(code);
}

// ----------------------------------------------------------------------------------------- //

PyCases PyCases::all_cases() noexcept {
    return from_ref(AllCases::instance().fetch());
}

std::string PyCases::repr(const PyCases &cases) noexcept {
    return std::format("<klotski.Cases size={}>", cases.size());
}

// ----------------------------------------------------------------------------------------- //

PyCases PyCases::from(RangesUnion &&data) noexcept {
    return PyCases(std::move(data));
}

PyCases PyCases::from_ref(const RangesUnion &data) noexcept {
    return PyCases(data);
}

const RangesUnion& PyCases::data_ref() const noexcept {
    return std::visit([]<typename T>(T &&arg) -> const RangesUnion& {
        if constexpr (std::is_same_v<std::decay_t<T>, RangesUnion>) {
            return arg;
        } else {
            return arg.get();
        }
    }, data_);
}

PyCases::PyCases(RangesUnion &&data) : data_(std::move(data)) {}
PyCases::PyCases(const RangesUnion &data) : data_(std::cref(data)) {}

// ----------------------------------------------------------------------------------------- //
