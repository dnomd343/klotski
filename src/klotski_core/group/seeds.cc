#include "group.h"
#include "type_id.h"
#include "group_seeds.h"

#include <iostream>

namespace klotski {

CommonCode Group::group_seed(uint32_t type_id, uint32_t group_id) {

    // TODO: check value

    auto offset = TYPE_ID_OFFSET[type_id];

    std::cout << "size: " << TYPE_ID_GROUP_NUM[type_id] << std::endl;

    auto k = GROUP_SEEDS_INDEX[offset + group_id];

    std::cout << "tmp index: " << k << std::endl;

    auto r = k + offset;

    std::cout << "real index: " << r << std::endl;

    auto seed = CommonCode(GROUP_SEEDS[r]);

    std::cout << "seed: " << seed << std::endl;

    std::cout << RawCode(seed) << std::endl;

    return seed;
}

} // namespace klotski
