#include "common_code.h"

using klotski::CommonCode;
using klotski::CommonCodeException;

inline uint8_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55'55'55'55;
    bin = (bin & 0x33'33'33'33) + ((bin >> 2) & 0x33'33'33'33);
    bin = ((bin >> 4) + bin) & 0x0F'0F'0F'0F;
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b11'11'11;
}

/// NOTE: input should not be zero
inline uint8_t last_zero_num(uint32_t bin) { // get last zero number
    bin ^= (bin - 1);
    return binary_count(bin >> 1);
}

CommonCode CommonCode::from_string(const std::string &common_code) {
    return CommonCode(common_code); // load from string
}

std::string CommonCode::to_string(bool shorten) const { // convert uint64_t code to string
    char result[10]; // max length 9-bits
    sprintf(result, "%09lX", code);
    if (shorten) { // remove `0` after common code
        if ((uint32_t)code == 0x00'00'00'00) { // low 32-bits are zero
            result[1] = '\0'; // only keep first character
            return result;
        }
        result[9 - last_zero_num(code) / 4] = '\0'; // truncate string
    }
    return result; // char* -> std::string
}

CommonCode::CommonCode(const std::string &common_code) { // convert from 1 ~ 9 bits string
    /// check string length
    if (common_code.length() > 9 || common_code.empty()) { // check string length
        throw CommonCodeException("common code should length 1 ~ 9");
    }
    /// check every characters
    uint64_t result = 0;
    for (auto const &bit : common_code) {
        result <<= 4;
        if (bit >= '0' && bit <= '9') { // 0 ~ 9
            result |= (bit - 48);
        } else if (bit >= 'A' && bit <= 'F') { // A ~ F
            result |= (bit - 55);
        } else if (bit >= 'a' && bit <= 'f') { // a ~ f
            result |= (bit - 87);
        } else {
            throw CommonCodeException("common code with invalid character"); // unknown character
        }
    }
    result <<= (9 - common_code.length()) * 4; // low-bits fill with zero
    /// check whether common code is valid
    if (!CommonCode::check(result)) { // check converted common code
        throw CommonCodeException("common code invalid");
    }
    code = result;
}
