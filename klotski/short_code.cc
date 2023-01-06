#include <stdexcept>
#include "common.h"
#include "all_cases.h"
#include "short_code.h"
#include "short_code_mark.h"
#include "common_code.h"

ShortCode::ShortCode(ShortCode::Mode mode) { // class initialize
    speed_up(mode);
}

bool ShortCode::check(uint32_t short_code) {
    return short_code < ShortCode::SHORT_CODE_LIMIT; // 0 ~ (SHORT_CODE_LIMIT - 1)
}

std::string ShortCode::code_to_string(uint32_t short_code) { // encode as 5-bits string
    if (!ShortCode::check(short_code)) {
        throw std::range_error("short code out of range");
    }
    std::string result(5, '\0'); // short code length 5
    for (int n = 0; n < 5; ++n) {
        uint8_t bit = short_code % 32;
        short_code = (short_code - bit) / 32;
        result[4 - n] = SHORT_CODE_TABLE[bit];
    }
    return result;
}

uint32_t ShortCode::code_from_string(const std::string &short_code) { // 5-bits string decode as number
    if (short_code.length() != 5) {
        throw std::runtime_error("invalid short code");
    }
    uint32_t result = 0;
    for (auto bit : short_code) {
        result *= 32;
        if (bit >= '1' && bit <= '9') { // 1 ~ 9
            result += bit - 49;
            continue;
        }
        if (bit >= 'a' && bit <= 'z') { // a ~ z
            bit -= ('a' - 'A'); // convert to A ~ Z
        }
        if (bit >= 'A' && bit <= 'Z') { // A ~ Z
            bit = SHORT_CODE_TABLE_REV[bit - 65]; // table convert
            if (bit == 0) {
                throw std::runtime_error("invalid short code");
            }
            result += bit;
        } else {
            throw std::runtime_error("invalid short code");
        }
    }
    if (!ShortCode::check(result)) {
        throw std::range_error("short code out of range");
    }
    return result;
}

void ShortCode::speed_up(enum Mode mode) { // speed up handle short code
    switch (mode) {
        case Mode::NORMAL: // speed up into normal mode
            if (basic_ranges.empty()) {
                build_base_ranges(); // basic ranges initialize
            }
            break;
        case Mode::FAST: // speed up into fast mode
            if (all_cases_list.empty()) {
                build_mappings(); // all cases mapping initialize
            }
            break;
    }
}

void ShortCode::build_base_ranges() { // build basic ranges
    auto all = AllCases(AllCases::Build::BASIC_RANGES);
    basic_ranges = *all.get_basic_ranges();
}

void ShortCode::build_mappings() { // build fast search mappings
    auto all = AllCases(AllCases::Build::ALL_CASES);
    for (int head = 0; head < 16; ++head) {
        uint64_t prefix = (uint64_t)head << 32;
        for (const auto &range : (*all.get_all_cases())[head]) {
            all_cases_list.emplace_back(prefix | range); // short_code => common_code
        }
    }
    for (int n = 0; n < all_cases_list.size(); ++n) {
        all_cases_dict[all_cases_list[n]] = n; // common_code => short_code
    }
}

enum ShortCode::Mode ShortCode::check_mode() { // ensure speed up enabled and return current mode
    if (!all_cases_list.empty()) {
        return ShortCode::Mode::FAST; // fast mode already enabled
    }
    if (!basic_ranges.empty()) {
        return ShortCode::Mode::NORMAL; // normal mode already enabled
    }
    speed_up(ShortCode::Mode::NORMAL); // class without initialized -> enter normal mode
    return ShortCode::Mode::NORMAL; // use normal mode
}

uint32_t ShortCode::zip_short_code(uint64_t common_code) { // common_code --zip--> short_code
    if (!CommonCode::check(common_code)) {
        throw std::runtime_error("invalid common code");
    }
    switch (check_mode()) {
        case ShortCode::Mode::NORMAL:
            return tiny_encode(common_code);
        case ShortCode::Mode::FAST:
            return all_cases_dict[common_code];
        default:
            throw std::runtime_error("unknown error");
    }
}

uint64_t ShortCode::unzip_short_code(uint32_t short_code) { // short_code --unzip--> common_code
    if (!ShortCode::check(short_code)) {
        throw std::runtime_error("invalid short code");
    }
    switch (check_mode()) {
        case ShortCode::Mode::NORMAL:
            return tiny_decode(short_code);
        case ShortCode::Mode::FAST:
            return all_cases_list[short_code];
        default:
            throw std::runtime_error("unknown error");
    }
}

uint32_t ShortCode::tiny_encode(uint64_t common_code) { // common_code --low-memory--> short_code
    uint32_t offset = 0;
    uint32_t head = common_code >> 32; // common code head
    uint32_t prefix = (common_code >> 24) & 0xFF; // common code range prefix
    auto target = Common::range_reverse((uint32_t)common_code); // target range

    for (int index = 0; index < BASIC_RANGES_INDEX[prefix]; ++index) { // traverse basic ranges
        uint32_t range = basic_ranges[index + BASIC_RANGES_OFFSET[prefix]];
        if (range == target) {
            break; // found target range
        }
        if (Common::check_case(head, range)) { // search for valid cases
            ++offset; // record sub offset
        }
    }
    return ALL_CASES_OFFSET[head] + RANGE_PREFIX_OFFSET[head][prefix] + offset;
}

uint64_t ShortCode::tiny_decode(uint32_t short_code) { // short_code --low-memory--> common_code
    uint32_t head = 0, prefix = 0;
    for (; head < 16; ++head) {
        if (short_code < ALL_CASES_INDEX[head]) { // match head
            break;
        }
        short_code -= ALL_CASES_INDEX[head]; // short code approximate
    }
    for (; prefix < 256; ++prefix) {
        if (short_code < RANGE_PREFIX_INDEX[head][prefix]) { // match range prefix
            break;
        }
        short_code -= RANGE_PREFIX_INDEX[head][prefix]; // short code approximate
    }

    uint32_t range;
    for (int index = 0; index < BASIC_RANGES_INDEX[prefix]; ++index) { // traverse basic ranges
        range = basic_ranges[index + BASIC_RANGES_OFFSET[prefix]];
        if (Common::check_case(head, range)) { // search for valid cases
            if (short_code == 0) {
                break; // found target range
            }
            --short_code; // short code approximate
        }
    }
    return (uint64_t)head << 32 | Common::range_reverse(range); // release common code
}
