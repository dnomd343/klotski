#pragma once

#include <string>
#include <cstdint>
#include <ostream>
#include "common_code.h"

class CommonCode;

class RawCode {
public:
    uint64_t unwrap() const;
    std::string dump_case() const;
    CommonCode to_common_code() const;

    static bool check(uint64_t raw_code);
    friend std::ostream& operator<<(std::ostream &out, const RawCode &self);

    explicit RawCode(uint64_t raw_code);
    explicit RawCode(const CommonCode &common_code);

    static RawCode create(uint64_t raw_code);
    static RawCode unsafe_create(uint64_t raw_code);
    static RawCode from_common_code(const CommonCode &common_code);

    // TODO: mirror functions

private:
    uint64_t code;
    RawCode() = default; // unsafe initialize

    static uint64_t compact(uint64_t raw_code);
    static uint64_t extract(uint64_t common_code);
};
