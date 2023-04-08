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

    static block_num_t block_num(const RawCode &raw_code);
    static block_num_t block_num(const CommonCode &common_code);

};

}
