/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

#include <string>
#include <exception>

namespace klotski::ffi {

class PyExc_CodecError final : std::exception {
public:
    explicit PyExc_CodecError(const std::string_view &msg) : msg_(msg) {}

    ~PyExc_CodecError() override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

} // namespace klotski::ffi