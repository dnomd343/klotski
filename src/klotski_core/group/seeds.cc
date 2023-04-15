#include "group.h"
#include "type_id.h"
#include "group_seeds.h"

#include <iostream>

namespace klotski {



CommonCode Group::group_seed(const GroupId &group_id) {

    // TODO: check value

    auto type_id = group_id.type_id();

    auto offset = TYPE_ID_OFFSET[type_id];

    std::cout << "size: " << TYPE_ID_GROUP_NUM[type_id] << std::endl;

    auto k = GROUP_SEEDS_INDEX[offset + group_id.unwrap()];

    std::cout << "tmp index: " << k << std::endl;

    auto r = k + offset;

    std::cout << "real index: " << r << std::endl;

    auto seed = CommonCode(GROUP_SEEDS[r]);

    std::cout << "seed: " << seed << std::endl;

    std::cout << RawCode(seed) << std::endl;

    return seed;
}

std::vector<CommonCode> Group::group_seeds(const TypeId &type_id) {
    auto offset = GROUP_SEEDS + TYPE_ID_OFFSET[type_id.unwrap()]; // group id offset
    return {offset, offset + TYPE_ID_GROUP_NUM[type_id.unwrap()]};
}



} // namespace klotski
