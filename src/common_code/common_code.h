#pragma once

#include <string>
#include <cstdint>

class CommonCode {
public:

    explicit CommonCode(uint64_t common_code);

    uint64_t unwrap() const;

    std::string to_string(bool shorten = false) const;

    static CommonCode from_string(const std::string &common_code);


private:

    uint64_t code;

    static bool check(uint64_t common_code);

//    static uint64_t from_string(const std::string &common_code);
//    static std::string to_string(uint64_t common_code, bool shorten = false);


};
