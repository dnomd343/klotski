#pragma once

#include <string>
#include <cstdint>

class CommonCode {
public:
    explicit CommonCode(uint64_t common_code);
    explicit CommonCode(const std::string &common_code_str);

    uint64_t unwrap() const;
    static bool check(uint64_t common_code);
    std::string to_string(bool shorten = false) const;

private:
    uint64_t code;
};
