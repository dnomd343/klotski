#include <algorithm>
#include "group.h"
#include "type_id.h"
#include "group_seeds.h"

#include <iostream>

namespace klotski {

Group::group_info_t Group::group_info(const RawCode &raw_code) {

    // TODO: function body

    return Group::group_info_t();
}

Group::group_info_t Group::group_info(const CommonCode &common_code) {

    auto type_id = Group::type_id(common_code);

    std::cout << type_id << std::endl;
    std::cout << "group num: " << TYPE_ID_GROUP_NUM[type_id] << std::endl;
    std::cout << "offset: " << TYPE_ID_OFFSET[type_id] << std::endl;

    uint32_t start = TYPE_ID_OFFSET[type_id];
    uint32_t end = start + TYPE_ID_GROUP_NUM[type_id];
    std::cout << "range: [" << start << ", " << end << ")" << std::endl;

    auto group = Group::group_cases(common_code);
    std::vector<CommonCode> g(group.begin(), group.end());
    auto seed = std::min_element(g.begin(), g.end());
    std::cout << "seed: " << *seed << std::endl;

    auto t = std::lower_bound(GROUP_SEEDS + start, GROUP_SEEDS + end, seed->unwrap());
    auto tmp_index = t - GROUP_SEEDS;
    std::cout << "tmp index: " << tmp_index;
    std::cout << " (" << CommonCode(GROUP_SEEDS[tmp_index]) << ")" << std::endl;

    auto group_id = GROUP_SEEDS_INDEX_[tmp_index];
    std::cout << "group id: " << group_id << std::endl;

    // TODO: function body

    return Group::group_info_t();
}

CommonCode Group::group_case(const Group::group_info_t &group_info) {

    // TODO: check group info

    // TODO: function body

    return CommonCode(0);
}

} // namespace klotski
