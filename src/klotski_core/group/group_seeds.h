#pragma once

#include <cstdint>

namespace klotski {

const uint64_t GROUP_SEEDS[25422] = {
#include "static/group_seeds.inc"
};

const uint16_t GROUP_SEEDS_INDEX[25422] = {
#include "static/group_seeds_index.inc"
};

const uint16_t GROUP_SEEDS_INDEX_REV[25422] = {
#include "static/group_seeds_index_rev.inc"
};

} // namespace klotski
