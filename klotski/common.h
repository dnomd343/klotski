#pragma once

#include <cstdint>

class Common {
public:
    static uint32_t range_reverse(uint32_t bin);
    static bool check_case(uint32_t head, uint32_t range);
    static bool check_case_safe(uint32_t head, uint32_t range);
};
