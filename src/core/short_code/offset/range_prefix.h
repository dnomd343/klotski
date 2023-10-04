#pragma once

/// This is the index for the range prefix. Given the case `head` and the first
/// 12-bit (0 ~ 4095) of the `range`, you can get the positions of all cases in
/// the current `head`. That is, according to the position of 2x2 block and the
/// `range`, the short code range is obtained.

#include <cstdint>

namespace klotski {
namespace codec {
namespace offset {

// TODO: using std::array
const uint32_t RANGE_PREFIX_OFFSET[16][4096] = {{
#include "range_prefix/offset_0x0.inc"
}, {
#include "range_prefix/offset_0x1.inc"
}, {
#include "range_prefix/offset_0x2.inc"
}, {
/// --------------- 0x3 ---------------
}, {
#include "range_prefix/offset_0x4.inc"
}, {
#include "range_prefix/offset_0x5.inc"
}, {
#include "range_prefix/offset_0x6.inc"
}, {
/// --------------- 0x7 ---------------
}, {
#include "range_prefix/offset_0x8.inc"
}, {
#include "range_prefix/offset_0x9.inc"
}, {
#include "range_prefix/offset_0xA.inc"
}, {
/// --------------- 0xB ---------------
}, {
#include "range_prefix/offset_0xC.inc"
}, {
#include "range_prefix/offset_0xD.inc"
}, {
#include "range_prefix/offset_0xE.inc"
}, {
/// --------------- 0xF ---------------
}};

} // namespace offset
} // namespace codec
} // namespace klotski
