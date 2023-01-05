#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "all_cases.h"
#include "short_code_mark.h"

// TODO: try to remove: `0` `O` `I` `l`

const uint32_t ALL_CASES_NUMBER = 29334498;

std::string code_to_string(uint32_t short_code) {
    if (short_code >= ALL_CASES_NUMBER) {
        throw std::range_error("short code out of range");
    }
    std::string result(5, '\0'); // short code length 5
    for (int i = 0; i < 5; ++i) {
        uint8_t bit = short_code % 36;
        short_code = (short_code - bit) / 36;
        if (bit < 10) {
            result[4 - i] = char(bit + 48); // 0 ~ 9
        } else {
            result[4 - i] = char(bit + 55); // A ~ Z
        }
    }
    return result;
}

uint32_t code_from_string(const std::string &short_code) {
    if (short_code.length() != 5) {
        throw std::runtime_error("invalid short code");
    }
    uint32_t result = 0;
    for (auto &bit : short_code) {
        result *= 36;
        if (bit >= '0' && bit <= '9') {
            result += bit - 48; // 0 ~ 9
        } else if (bit >= 'A' && bit <= 'Z') {
            result += bit - 55; // A ~ Z
        } else if (bit >= 'a' && bit <= 'z') {
            result += bit - 87; // a ~ z
        } else {
            throw std::runtime_error("invalid short code");
        }
    }
    if (result >= ALL_CASES_NUMBER) {
        throw std::range_error("short code out of range");
    }
    return result;
}

uint64_t unzip_short_code(uint32_t short_code) {

    auto a = AllCases(); // load basic ranges

    std::cout << "short code: " << short_code << std::endl;

    uint32_t head = 0;
    for (; head < 16; ++head) {
        if (short_code < ALL_CASES_INDEX[head]) {
            break;
        }
        short_code -= ALL_CASES_INDEX[head];
    }

    std::cout << "head: " << head << std::endl;
    std::cout << "short code: " << short_code << std::endl;

    uint32_t prefix = 0;
    for (; prefix < 256; ++prefix) {
        if (short_code < SHORT_CODE_INDEX[head][prefix]) {
            break;
        }
        short_code -= SHORT_CODE_INDEX[head][prefix];
    }

    std::cout << "prefix: " << prefix << std::endl;
    std::cout << "short code: " << short_code << std::endl;

    uint32_t range;
    for (int i = 0; i < BASIC_RANGES_INDEX[prefix]; ++i) {
        range = a.basic_ranges[i + BASIC_RANGES_OFFSET[prefix]];
        if (AllCases::check_case(head, range)) {
            if (short_code == 0) {
                break;
            }
            --short_code;
        }
    }
    return (uint64_t)head << 32 | AllCases::binary_reverse(range);
}

uint32_t zip_short_code(uint64_t code) {
    auto a = AllCases(); // load basic ranges

    uint32_t head = code >> 32;
    uint32_t head_offset = ALL_CASES_OFFSET[head];

    std::cout << "head: " << head << std::endl;
    std::cout << "head offset: " << head_offset << std::endl;

    uint32_t prefix = (code >> 24) & 0xFF;
    uint32_t prefix_offset = SHORT_CODE_OFFSET[head][prefix];

    std::cout << "prefix: " << prefix << std::endl;
    std::cout << "prefix offset: " << prefix_offset << std::endl;

    uint32_t basic_index = BASIC_RANGES_INDEX[prefix];
    uint32_t basic_offset = BASIC_RANGES_OFFSET[prefix];

    std::cout << "basic index: " << basic_index << std::endl;
    std::cout << "basic offset: " << basic_offset << std::endl;

    auto target_range = AllCases::binary_reverse((uint32_t)code);
    printf("target range -> %08X\n", target_range);

    uint32_t sub_offset = 0;
    for (int i = 0; i < basic_index; ++i) {
        uint32_t range = a.basic_ranges[i + basic_offset];
        if (range == target_range) {
            break;
        }
        if (AllCases::check_case(head, range)) {
            ++sub_offset;
        }
    }
    std::cout << "sub offset: " << sub_offset << std::endl;

    return head_offset + prefix_offset + sub_offset;
}

int main() {

//    auto ret_code = unzip_short_code(14323231);
//    printf("result -> %08lX\n", ret_code);

    auto ret_code = zip_short_code(0x6EC0F8800);
    printf("result -> %d\n", ret_code);

    return 0;

    auto a = AllCases();
//    a.find_all_cases();

//    std::vector<uint64_t> all_cases;
//    for (int n = 0; n < 16; ++n) {
//        uint64_t prefix = (uint64_t)n << 32;
//        for (auto &code : a.all_cases[n]) {
//            all_cases.emplace_back(prefix | code);
//        }
//    }
//
//    std::unordered_map<uint64_t, uint32_t> all_cases_dict;
//    for (int i = 0; i < all_cases.size(); ++i) {
//        all_cases_dict[all_cases[i]] = i;
//    }

//    uint32_t short_code = 14323231;
//    printf("%09lX\n", all_cases[0]);
//    printf("%09lX\n", all_cases[short_code]);
//    printf("%09lX\n", all_cases[ALL_CASES_NUMBER - 1]);

//    std::cout << code_to_string(all_cases_dict[0x6EC0F8800]) << std::endl;
//    std::cout << code_to_string(all_cases_dict[0x1A9BF0C00]) << std::endl;
//    std::cout << code_to_string(all_cases_dict[0x4FEA13400]) << std::endl;

//    std::cout << code_to_string(14323231) << std::endl;
//    std::cout << code_from_string("8IzVj") << std::endl;

//    uint32_t offset[16];
//    uint32_t sum = 0;
//    for (int i = 0; i < 16; ++i) {
//        offset[i] = sum;
//        sum += ALL_CASES_INDEX[i];
//    }
//    for (int i = 0; i < 16; ++i) {
//        std::cout << offset[i] << ", ";
//    }

//    uint32_t offset[256];
//
//    for (uint32_t head = 0; head < 16; ++head) {
//
//        uint32_t sum = 0;
//        for (int i = 0; i < 256; ++i) {
//            offset[i] = sum;
//            sum += SHORT_CODE_INDEX[head][i];
//        }
//        std::cout << "{" << std::endl;
//        for (int i = 0; i < 256; ++i) {
//            if (i % 8 == 0) {
//                std::cout << "    ";
//            }
////            printf("%7d, ", offset[i]);
//            printf("%5d, ", SHORT_CODE_INDEX[head][i]);
//            if (i % 8 == 7) {
//                std::cout << std::endl;
//            }
//        }
//        std::cout << "}," << std::endl;
//    }

    return 0;
}
