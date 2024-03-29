#pragma once

/// This is the head index, the offset (0 ~ 29334498) in all cases is obtained
/// according to the `head` (0 ~ 15). In other words, the short code range can be
/// obtained according to the position of the 2x2 block.

#include <cstdint>

namespace klotski {

const uint32_t ALL_CASES_OFFSET[16] = {
           0,  2942906,  5203298,  8146204,
     8146204, 10468254, 12345199, 14667249,
    14667249, 16989299, 18866244, 21188294,
    21188294, 24131200, 26391592, 29334498,
};

} // namespace klotski
