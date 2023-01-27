#include "all_cases.h"
#include "short_code.h"

namespace std {
    template<>
    struct hash<klotski::ShortCode> {
        std::size_t operator()(const klotski::ShortCode &c) const {
            return std::hash<uint64_t>()(c.unwrap());
        }
    };

    template<>
    struct equal_to<klotski::ShortCode> {
        bool operator()(const klotski::ShortCode &c1, const klotski::ShortCode &c2) const {
            return c1.unwrap() == c2.unwrap();
        }
    };
}

namespace klotski {
    bool ShortCode::operator==(const ShortCode &short_code) const {
        return this->code == short_code.code;
    }

    std::ostream &operator<<(std::ostream &out, const ShortCode &self) {
        out << self.to_string() << "(" << self.code << ")"; // short code info
        return out;
    }
}

namespace klotski {
    bool ShortCode::valid() const {
        return ShortCode::check(code);
    }

    ShortCode ShortCode::create(uint32_t short_code) {
        return ShortCode(short_code);
    }

    ShortCode ShortCode::unsafe_create(uint32_t short_code) { // create without check
        auto tmp = ShortCode(); // init directly
        tmp.code = short_code;
        return tmp;
    }

    ShortCode::ShortCode(uint32_t short_code) {
        if (!ShortCode::check(short_code)) { // check input short code
            throw std::invalid_argument("invalid short code");
        }
        code = short_code;
    }
}

bool klotski::ShortCode::check(uint32_t short_code) {
    return short_code < SHORT_CODE_LIMIT; // 0 ~ (SHORT_CODE_LIMIT - 1)
}

using klotski::ShortCode;

bool ShortCode::fast_mode_available = false;
bool ShortCode::normal_mode_available = false;

ShortCode::Mode ShortCode::mode() { // ensure speed up enabled and return current mode
    if (fast_mode_available) {
        return ShortCode::FAST; // fast mode already enabled
    }
    if (normal_mode_available) {
        return ShortCode::NORMAL; // normal mode already enabled
    }
    speed_up(ShortCode::Mode::NORMAL); // uninitialized -> enable normal mode
    return ShortCode::Mode::NORMAL; // normal mode enabled
}

void ShortCode::speed_up(ShortCode::Mode mode) {
    if (fast_mode_available) {
        return; // fast mode already available
    }
    if (mode == ShortCode::FAST) { // build fast mode data
        AllCases::build(); // blocking function
        fast_mode_available = true;
    } else if (!normal_mode_available) { // build normal mode data
        BasicRanges::build(); // blocking function
        normal_mode_available = true;
    }
}
