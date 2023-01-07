#pragma once

#include <string>
#include <cstdint>

class CommonCode {
public:
    static bool check(uint64_t common_code);
//    static uint64_t code_from_string(const std::string &common_code);
    static std::string code_to_string(uint64_t common_code, bool shorten = false);
};
