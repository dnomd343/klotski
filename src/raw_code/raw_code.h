#pragma once

#include <cstdint>
#include "common_code.h"

class CommonCode;

class RawCode {
public:
    uint64_t unwrap() const;
    std::string dump_case() const;
    CommonCode to_common_code() const;

    explicit RawCode(const CommonCode &common_code);
    explicit RawCode(uint64_t raw_code) : code(raw_code) {}

private:
    uint64_t code;
};
