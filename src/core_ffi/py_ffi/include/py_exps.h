#pragma once

#include <exception>

namespace klotski::ffi {

class PyCodecExp final : std::exception {
public:
    explicit PyCodecExp(const std::string_view &msg) : msg_(msg) {}

    ~PyCodecExp() override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

} // namespace klotski::ffi
