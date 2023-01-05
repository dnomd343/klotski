#include <iostream>
#include <algorithm>
#include "all_cases.h"

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

int main() {

//    auto a = AllCases();
//    a.find_all_cases();
//    int sum = 0;
//    for (auto &all_case : a.all_cases) {
//        sum += (int)all_case.size();
//    }
//    std::cout << sum << std::endl;

    std::cout << code_to_string(14323231) << std::endl;
    std::cout << code_from_string("8IzVj") << std::endl;

    return 0;
}
