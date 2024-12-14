#pragma once

#include "raw_code/raw_code.h"

namespace helper {

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

RawCode get_vertical_mirror(RawCode code);

RawCode get_horizontal_mirror(RawCode code);

CommonCode get_vertical_mirror(CommonCode code);

CommonCode get_horizontal_mirror(CommonCode code);

} // namespace helper
