/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <iostream>

#include <variant>

#include "ranges/ranges.h"

#include "py_common_code.h"

namespace klotski::ffi {

using klotski::cases::RangesUnion;

//class PyCases {
//public:
//    PyCases() = delete;
//
//    PyCases(PyCases &&cases) noexcept : real_data_(std::move(cases.real_data_)), data_(real_data_) {
//        std::cout << "PyCases(PyCases &&) called" << std::endl;
//        std::cout << "real_data / data_ref -> " << &real_data_ << " " << &data_ << std::endl;
//    }
//
//    PyCases(const PyCases &cases) : real_data_(cases.real_data_), data_(real_data_) {
//        std::cout << "PyCases(const PyCases &) called" << std::endl;
//        std::cout << "real_data / data_ref -> " << &real_data_ << " " << &data_ << std::endl;
//    }
//
//    PyCases& operator==(PyCases &&cases) {
//        std::cout << "PyCases& operator==(PyCases &&) called" << std::endl;
//        if (this != &cases) {
//            real_data_ = std::move(cases.real_data_);
//        }
//        std::cout << "real_data / data_ref -> " << &real_data_ << " " << &data_ << std::endl;
//        return *this;
//    }
//
//    PyCases& operator==(const PyCases &&cases) {
//        std::cout << "PyCases& operator==(const PyCases &&) called" << std::endl;
//        if (this != &cases) {
//            real_data_ = cases.real_data_;
//        }
//        std::cout << "real_data / data_ref -> " << &real_data_ << " " << &data_ << std::endl;
//        return *this;
//    }
//
//    [[nodiscard]] size_t size() const;
//
//    static PyCases from(RangesUnion &&data);
//
//    static PyCases from_ref(const RangesUnion &data);
//
//    [[nodiscard]] const RangesUnion& data_ref() const;
//
//private:
//    explicit PyCases(RangesUnion &&data) : real_data_(std::move(data)), data_(real_data_) {
//        std::cout << "PyCases init from r-value" << std::endl;
//        std::cout << "real_data / data_ref -> " << &real_data_ << " " << &data_ << std::endl;
//    }
//
//    explicit PyCases(const RangesUnion &data) : data_(data) {
//        std::cout << "PyCases init from l-value" << std::endl;
//    }
//
//    RangesUnion real_data_;
//    const RangesUnion &data_;
//};

class PyCases {
public:
    using Cases = RangesUnion;
    using CasesRef = std::reference_wrapper<const Cases>;

    static PyCases from(RangesUnion &&data) {
        return PyCases(std::move(data));
    }

    static PyCases from_ref(const RangesUnion &data) {
        return PyCases(data);
    }

    [[nodiscard]] size_t size() const;

    [[nodiscard]] const Cases& ref() const {

        // if (std::holds_alternative<Cases>(data_)) {
        //     return std::get<Cases>(data_);
        // } else {
        //     return std::get<CasesRef>(data_);
        // }

        return std::visit([]<typename T>(T &&arg) -> const Cases& {
            if constexpr (std::is_same_v<std::decay_t<T>, Cases>) {
                return arg;
            } else {
                return arg.get();
            }
        }, data_);
    }

private:
    std::variant<Cases, CasesRef> data_;

    explicit PyCases(const RangesUnion &data) : data_(std::cref(data)) {
        std::cout << "PyCases(const RangesUnion &) called" << std::endl;
        std::cout << "variant data index: " << data_.index() << std::endl;
    }

    explicit PyCases(RangesUnion &&data) : data_(std::move(data)) {
        std::cout << "PyCases(RangesUnion &&) called" << std::endl;
        std::cout << "variant data index: " << data_.index() << std::endl;
    }
};

class PyCasesIter {
public:
    explicit PyCasesIter(const PyCases &data) : data_(data) {}

    PyCommonCode next();

private:
    uint64_t head_ {0};
    size_t index_ {0};
    const PyCases &data_;
};

} // namespace klotski::ffi
