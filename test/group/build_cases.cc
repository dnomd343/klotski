#include <algorithm>
#include "md5.h"
#include "group.h"
#include "common_code.h"
#include "gtest/gtest.h"

#include "tiny_pool.h"

#include "group/type_id.h"
#include "group/group_seeds.h"

using klotski::Group;
using klotski::TypeId;
using klotski::AllCases;

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::GROUP_SEEDS;
using klotski::TYPE_ID_SIZE;
using klotski::TYPE_ID_LIMIT;
using klotski::SHORT_CODE_LIMIT;
using klotski::ALL_CASES_SIZE_SUM;

const char GROUP_INFO_MD5[] = "976bf22530085210e68a6a4e67053506";

TEST(Group, all_cases) {
    std::vector<std::vector<CommonCode>> all_cases;
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        all_cases.emplace_back(Group::all_cases(type_id)); // build test data
    }

    std::vector<uint64_t> combine;
    combine.reserve(ALL_CASES_SIZE_SUM);
    for (uint32_t id = 0; id < TYPE_ID_LIMIT; ++id) {
        EXPECT_EQ(all_cases[id].size(), TYPE_ID_SIZE[id]); // verify cases number
        for (auto &&common_code : all_cases[id]) {
            EXPECT_EQ(TypeId(common_code).unwrap(), id); // verify type id
            combine.emplace_back(common_code.unwrap());
        }
        std::is_sorted(all_cases[id].begin(), all_cases[id].end()); // verify data order
    }
    EXPECT_EQ(combine.size(), ALL_CASES_SIZE_SUM); // verify sum

    auto all_cases_release = AllCases::release();
    std::stable_sort(combine.begin(), combine.end());
    for (uint32_t i = 0; i < combine.size(); ++i) {
        EXPECT_EQ(combine[i], all_cases_release[i]); // verify after combined
    }
}

TEST(Group, group_cases) {
    auto test = [](CommonCode seed) -> std::vector<CommonCode> {
        auto group_raw = Group::group_cases(RawCode::from_common_code(seed));
        std::vector<CommonCode> group(group_raw.begin(), group_raw.end()); // convert as CommonCodes
        EXPECT_EQ(seed, std::min_element(group.begin(), group.end())->unwrap()); // confirm min seed

        uint32_t type_id = TypeId(CommonCode(seed)).unwrap(); // current type id
        for (auto &&elem : group) {
            EXPECT_EQ(TypeId(elem).unwrap(), type_id); // verify type id of group cases
        }
        return group;
    };

    auto pool = TinyPool();
    std::vector<std::future<std::vector<CommonCode>>> futures;
    for (auto &&seed : GROUP_SEEDS) {
        futures.emplace_back(
            pool.submit(test, CommonCode::unsafe_create(seed))
        );
    }

    pool.boot();
    std::vector<CommonCode> all_cases;
    all_cases.reserve(ALL_CASES_SIZE_SUM);
    for (auto &&f : futures) {
        auto ret = f.get();
        all_cases.insert(all_cases.end(), ret.begin(), ret.end());
    }
    std::sort(all_cases.begin(), all_cases.end());
    EXPECT_EQ(all_cases, AllCases::release()); // verify all released cases
}

TEST(Group, build_group) {

}

TEST(Group, build_groups) {
    struct group_info_t {
        uint16_t group_id;
        uint32_t group_index;
    };
    ShortCode::speed_up(ShortCode::FAST);
    std::vector<group_info_t> all_cases(SHORT_CODE_LIMIT);

    std::vector<uint16_t> group_ids;

    for (uint16_t type_id = 0; type_id < klotski::TYPE_ID_LIMIT; ++type_id) {
        std::vector<uint32_t> group_sizes;
        std::vector<CommonCode> group_seeds;
        auto groups = Group::build_groups(type_id);
        group_sizes.reserve(groups.size());
        group_seeds.reserve(groups.size());

        for (uint32_t id = 0; id < groups.size(); ++id) {
            group_sizes.emplace_back(groups[id].size()); // record size
            std::sort(groups[id].begin(), groups[id].end()); // sort for group index
            for (uint32_t index = 0; index < groups[id].size(); ++index) {
                all_cases[groups[id][index].to_short_code().unwrap()] = {
                    .group_id = static_cast<uint16_t>(id),
                    .group_index = index,
                };
                EXPECT_EQ(TypeId(groups[id][index]).unwrap(), type_id); // verify type id
            }
            group_seeds.emplace_back(groups[id][0]); // record seed
        }

        // TODO: check group_seeds and group_sizes

        // TODO: verify GROUP_OFFSET

    }

    // TODO: verify GROUP_SEEDS / GROUP_SEEDS_INDEX / GROUP_SEEDS_INDEX_REV

    char buffer[9];
    std::string group_info;
    for (auto &&tmp : all_cases) {
        sprintf(buffer, "%d,%d\n", tmp.group_id, tmp.group_index);
        group_info += buffer;
    }
    auto group_info_md5 = md5(group_info.c_str(), group_info.size());
    EXPECT_STREQ(group_info_md5.c_str(), GROUP_INFO_MD5); // verify all group info
}
