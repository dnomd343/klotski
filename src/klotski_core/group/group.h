#pragma once

#include <cstdint>
#include "raw_code.h"
#include "common_code.h"

namespace klotski {

class Group {
public:

    struct block_num_t {
        uint8_t n_1x1 = 0;
        uint8_t n_1x2 = 0;
        uint8_t n_2x1 = 0;
    };

//    static block_num_t block_num(uint32_t type_id);
    static block_num_t block_num(const RawCode &raw_code);
    static block_num_t block_num(const CommonCode &common_code);

//    static uint32_t max_group_size(uint32_t type_id);
    static uint32_t max_group_size(const RawCode &raw_code) {
        return 65535 * 8;
    };

    static std::vector<RawCode> group_cases(const RawCode &seed);

};

}
