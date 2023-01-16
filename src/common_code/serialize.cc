#include "common_code.h"

inline uint8_t binary_count(uint32_t bin) { // get number of non-zero bits
    bin -= (bin >> 1) & 0x55555555;
    bin = (bin & 0x33333333) + ((bin >> 2) & 0x33333333);
    bin = ((bin >> 4) + bin) & 0x0F0F0F0F;
    bin += bin >> 8;
    bin += bin >> 16;
    return bin & 0b111111;
}

/// NOTE: input should not be zero
inline uint32_t last_zero_num(uint32_t bin) { // get last zero number
    bin ^= (bin - 1);
    return binary_count(bin >> 1);
}

std::string CommonCode::to_string(bool shorten) const { // convert uint64_t code to string
    char result[10]; // max length 9-bits
    sprintf(result, "%09lX", code);
    if (shorten) { // remove `0` after common code
        if (code == 0x000000000) {
            return "0"; // special case -> only one `0`
        }
        result[9 - last_zero_num(code) / 4] = '\0'; // truncate string
    }
    return result; // char* -> std::string
}

CommonCode::CommonCode(const std::string &common_code) { // convert from 1 ~ 9 bits string
    /// check string length
    if (common_code.length() > 9 || common_code.empty()) { // check string length
        throw std::invalid_argument("common code format error");
    }
    /// check every characters
    uint64_t result = 0;
    for (auto const &bit : common_code) {
        result <<= 4;
        if (bit >= '0' && bit <= '9') { // 0 ~ 9
            result |= (bit - 48);
        } else if (bit >= 'A' && bit <= 'Z') { // A ~ Z
            result |= (bit - 55);
        } else if (bit >= 'a' && bit <= 'z') { // a ~ z
            result |= (bit - 87);
        } else {
            throw std::invalid_argument("common code format error"); // unknown characters
        }
    }
    result <<= (9 - common_code.length()) * 4; // low-bits fill with zero
    /// check whether common code is valid
    if (!CommonCode::check(result)) { // check converted common code
        throw std::invalid_argument("invalid common code");
    }
    code = result;
}
