#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "all_cases.h"
#include "mark.h"

// TODO: try to remove: `0` `O` `I` `l`

const uint32_t ALL_CASES_NUMBER = 29334498;

const uint32_t ALL_CASES_SIZE[] = {
    2942906, 2260392, 2942906, 0,
    2322050, 1876945, 2322050, 0,
    2322050, 1876945, 2322050, 0,
    2942906, 2260392, 2942906, 0,
};

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

int main() {

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

    uint32_t short_code = 14323231;
//    printf("%09lX\n", all_cases[0]);
//    printf("%09lX\n", all_cases[short_code]);
//    printf("%09lX\n", all_cases[ALL_CASES_NUMBER - 1]);

//    std::cout << code_to_string(all_cases_dict[0x6EC0F8800]) << std::endl;
//    std::cout << code_to_string(all_cases_dict[0x1A9BF0C00]) << std::endl;
//    std::cout << code_to_string(all_cases_dict[0x4FEA13400]) << std::endl;

    // located code header
    // 0 <= short_code < ALL_CASES_NUMBER

//    uint32_t short_code = 5203298;
    int head = 0;
    for (; head < 16; ++head) {
        if (short_code < ALL_CASES_SIZE[head]) {
            break;
        }
        short_code -= ALL_CASES_SIZE[head];
    }
    std::cout << "head: " << head << std::endl;
    std::cout << "sub short code: " << short_code << std::endl;

//    int index = 0;
//    for (auto &range : a.basic_ranges) {
//        if (AllCases::check_case(head, range)) {
//            if (index == short_code) {
//                printf("%08X\n", AllCases::binary_reverse(range));
//                break;
//            }
//            ++index;
//        }
//    }

    // group size => 256 => 8-bits

    // min range -> xx * 7 -> 14-bits

    // 00 00 00 00 ...  =>  ... 00 00 00 00
    // 00 00 00 01 ...  =>  ... 01 00 00 00
    // 00 00 00 10 ...  =>  ... 10 00 00 00
    // 00 00 00 11 ...  =>  ... 11 00 00 00
    // ...

    // ...00000000 -> 0
    // ...01000000 -> ...
    // ...10000000 -> ...
    // ...11000000 -> ...

    // record start search point


    int prefix = 0;
    for (; prefix < 256; ++prefix) {
        if (short_code < SHORT_CODE_MARK[head][prefix]) {
            break;
        }
        short_code -= SHORT_CODE_MARK[head][prefix];
    }
    std::cout << "prefix: " << prefix << std::endl;
    std::cout << "sub sub short code: " << short_code << std::endl;

    uint32_t basic_ranges_start = 0;
    for (int i = 0; i < prefix; ++i) {
        basic_ranges_start += BASIC_RANGES_INDEX[i];
    }
    std::cout << "basic ranges start: " << basic_ranges_start << std::endl;

    uint32_t range;
    for (int i = 0; i < BASIC_RANGES_INDEX[prefix]; ++i) {
        range = a.basic_ranges[i + basic_ranges_start];
        if (AllCases::check_case(head, range)) {
            if (short_code == 0) {
                break;
            }
            --short_code;
        }
    }
    range = AllCases::binary_reverse(range);
    printf("range -> %08X\n", range);
    printf("result code -> %X%08X\n", head, range);

//    std::cout << code_to_string(14323231) << std::endl;
//    std::cout << code_from_string("8IzVj") << std::endl;

    return 0;
}
