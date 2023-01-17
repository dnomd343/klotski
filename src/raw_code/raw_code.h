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
//    friend std::ostream& operator<<(std::ostream &out, const RawCode &self);

    explicit RawCode(const CommonCode &common_code);

    // TODO: check raw_code before object create
    explicit RawCode(uint64_t raw_code) : code(raw_code) {}

    // TODO: mirror functions


//    static RawCode create(uint64_t raw_code);
    static RawCode from_common_code(const CommonCode &common_code);

    // TODO: unsafe_create static function

private:
    uint64_t code;

    static uint64_t compact(uint64_t raw_code);
    static uint64_t extract(uint64_t common_code);
};
