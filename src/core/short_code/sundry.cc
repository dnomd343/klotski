#include "short_code.h"
#include "common_code.h"

namespace klotski {
namespace codec {

// FIXME: just for compile
CommonCode ShortCode::to_common_code() const noexcept {
    return CommonCode::unsafe_create(0);
}

// FIXME: just for compile
std::optional<ShortCode> ShortCode::from_string(std::string &&short_code) noexcept {
    return std::nullopt;
}

// FIXME: just for compile
std::optional<ShortCode> ShortCode::from_string(const std::string &short_code) noexcept {
    return std::nullopt;
}

} // namespace codec
} // namespace klotski
