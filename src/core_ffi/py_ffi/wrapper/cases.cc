#include <format>
#include <pybind11/pybind11.h>

#include "py_ffi/cases.h"

namespace py = pybind11;

using namespace klotski::ffi;

// ----------------------------------------------------------------------------------------- //

PyCasesIter::PyCasesIter(const RangesUnion &data) : data_(data) {}

PyCommonCode PyCasesIter::next() {
    while (head_ < 16) {
        const auto &ranges = data_.ranges(head_);
        if (index_ < ranges.size()) {
            auto code = (static_cast<uint64_t>(head_) << 32) | ranges[index_++];
            return std::bit_cast<PyCommonCode>(code);
        }
        index_ = 0, ++head_;
    }
    throw py::stop_iteration();
}

// ----------------------------------------------------------------------------------------- //

size_t PyCases::size() const noexcept {
    size_t num = 0;
    for (const auto &x : data_ref()) { // TODO: fetch from RangesUnion.size()
        num += x.size();
    }
    return num;
}

PyCasesIter PyCases::codes() const noexcept {
    return PyCasesIter(data_ref());
}

PyCommonCode PyCases::at(size_t index) const {
    if (index >= size()) {
        throw py::index_error("cases index out of range");
    }

    uint64_t head = 0;
    for (;;) {
        if (index >= data_ref().ranges(head).size()) {
            index -= data_ref().ranges(head).size();
            ++head;
        } else {
            break;
        }
    }
    uint32_t range = data_ref().ranges(head)[index];

    // TODO: fetch from RangesUnion[]
    const auto code = CommonCode::unsafe_create(head << 32 | range);
    return std::bit_cast<PyCommonCode>(code);
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
