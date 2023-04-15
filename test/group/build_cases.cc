#include <algorithm>
#include "md5.h"
#include "group.h"
#include "type_id.h"
#include "tiny_pool.h"
#include "common_code.h"
#include "group_seeds.h"
#include "gtest/gtest.h"

using klotski::Group;
using klotski::TypeId;
using klotski::GroupId;
using klotski::AllCases;

using klotski::RawCode;
using klotski::ShortCode;
using klotski::CommonCode;

using klotski::GROUP_SEEDS;
using klotski::TYPE_ID_SIZE;
using klotski::ALL_GROUP_NUM;
using klotski::TYPE_ID_LIMIT;
using klotski::SHORT_CODE_LIMIT;
using klotski::TYPE_ID_GROUP_NUM;
using klotski::ALL_CASES_SIZE_SUM;

const char GROUP_INFO_MD5[] = "976bf22530085210e68a6a4e67053506";

TEST(Group, all_cases) {
    std::array<std::vector<CommonCode>, TYPE_ID_LIMIT> all_cases;
    auto build = [&all_cases](TypeId type_id) {
        auto cases = type_id.all_cases(); // build test data
        EXPECT_EQ(cases.size(), TYPE_ID_SIZE[type_id.unwrap()]); // verify cases number
        for (auto &&common_code : cases) {
            EXPECT_EQ(TypeId(common_code), type_id); // verify type id
        }
        EXPECT_EQ(std::is_sorted(cases.begin(), cases.end()), true); // verify data order
        all_cases[type_id.unwrap()] = cases;
    };

    auto pool = TinyPool();
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        pool.submit(build, TypeId(type_id));
    }
    pool.boot();
    pool.join(); // wait data build complete

    std::vector<CommonCode> combine;
    combine.reserve(ALL_CASES_SIZE_SUM);
    for (auto &&cases : all_cases) {
        combine.insert(combine.end(), cases.begin(), cases.end());
    }
    EXPECT_EQ(combine.size(), ALL_CASES_SIZE_SUM); // verify sum

    auto all_cases_release = AllCases::release();
    std::stable_sort(combine.begin(), combine.end());
    EXPECT_EQ(combine, all_cases_release); // verify combined cases
}

TEST(Group, group_cases) {
    auto build = [](CommonCode seed) -> std::vector<CommonCode> {
        auto group_raw = Group::cases(seed);
        std::vector<CommonCode> group(group_raw.begin(), group_raw.end()); // convert as CommonCodes
        EXPECT_EQ(seed, std::min_element(group.begin(), group.end())->unwrap()); // confirm min seed
        EXPECT_EQ(group.size(), GroupId::size(seed)); // verify group size

        uint32_t type_id = TypeId(seed).unwrap(); // current type id
        for (auto &&elem : group) {
            EXPECT_EQ(TypeId(elem).unwrap(), type_id); // verify type id of group cases
        }
        return group;
    };

    auto pool = TinyPool();
    std::vector<std::future<std::vector<CommonCode>>> futures;
    for (auto &&seed : GROUP_SEEDS) {
        futures.emplace_back(
            pool.submit(build, CommonCode::unsafe_create(seed))
        );
    }

    pool.boot();
    std::vector<CommonCode> all_cases;
    all_cases.reserve(ALL_CASES_SIZE_SUM);
    for (auto &&f : futures) {
        auto cases = f.get();
        all_cases.insert(all_cases.end(), cases.begin(), cases.end()); // combine build result
    }
    std::sort(all_cases.begin(), all_cases.end());
    EXPECT_EQ(all_cases, AllCases::release()); // verify all released cases
}

TEST(Group, group_seeds) {
    std::vector<CommonCode> all_seeds;
    all_seeds.reserve(ALL_GROUP_NUM);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto seeds = TypeId(type_id).seeds();
        for (auto &&seed : seeds) {
            EXPECT_EQ(TypeId(seed).unwrap(), type_id); // verify type id of seeds
        }
        all_seeds.insert(all_seeds.end(), seeds.begin(), seeds.end());

        std::vector<CommonCode> sub_seeds;
        sub_seeds.reserve(TYPE_ID_GROUP_NUM[type_id]);
        for (uint32_t group_id = 0; group_id < TYPE_ID_GROUP_NUM[type_id]; ++group_id) {
            sub_seeds.emplace_back(GroupId(type_id, group_id).seed());
        }
        std::sort(seeds.begin(), seeds.end());
        std::sort(sub_seeds.begin(), sub_seeds.end()); // don't verify seeds order for now
        EXPECT_EQ(seeds, sub_seeds); // verify group seeds
    }
    std::vector<CommonCode> group_seeds(GROUP_SEEDS, GROUP_SEEDS + ALL_GROUP_NUM);
    EXPECT_EQ(all_seeds, group_seeds); // verify group seeds

    auto test = [](CommonCode seed) {
        EXPECT_EQ(GroupId::seed(seed), seed); // verify group seed fetch
        EXPECT_EQ(GroupId::seed(seed.to_raw_code()), seed);
    };
    auto pool = TinyPool();
    for (auto &&seed : GROUP_SEEDS) { // traverse all seeds
        pool.submit(test, CommonCode::unsafe_create(seed));
    }
    pool.boot();
    pool.join();
}

TEST(Group, build_groups) {
    std::vector<Group::group_info_t> all_cases(SHORT_CODE_LIMIT);

    auto test = [&all_cases](TypeId type_id) {
        auto groups = Group::build_groups(type_id);
        EXPECT_EQ(groups.size(), TYPE_ID_GROUP_NUM[type_id.unwrap()]); // verify groups num

        std::vector<uint32_t> group_sizes;
        std::map<uint32_t, std::vector<CommonCode>> group_seeds; // <group_size, group_seeds>

        for (uint32_t group_id = 0; group_id < groups.size(); ++group_id) {
            auto group = Group::build_group(GroupId(type_id, group_id));
            std::sort(group.begin(), group.end());
            std::sort(groups[group_id].begin(), groups[group_id].end());
            EXPECT_EQ(groups[group_id], group); // verify group data
            EXPECT_EQ(group.size(), GroupId(type_id, group_id).size()); // verify group size
            EXPECT_EQ(*group.begin(), GroupId(type_id, group_id).seed()); // verify group seed

            for (uint32_t index = 0; index < group.size(); ++index) {
                all_cases[group[index].to_short_code().unwrap()] = { // storage group info
                    .type_id = 0,
                    .group_id = static_cast<uint16_t>(group_id),
                    .group_index = index,
                };
                EXPECT_EQ(TypeId(group[index]), type_id); // verify released type id
            }

            group_seeds[group.size()].emplace_back(*group.begin()); // storage group seeds
            group_sizes.emplace_back(group.size()); // storage group size
        }

        EXPECT_EQ(std::is_sorted(group_sizes.rbegin(), group_sizes.rend()), true); // verify group size order
        for (auto &&tmp : group_seeds) {
            EXPECT_EQ(std::is_sorted(tmp.second.begin(), tmp.second.end()), true); // verify group cases order
        }
    };

    auto pool = TinyPool();
    ShortCode::speed_up(ShortCode::FAST);
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        pool.submit(test, TypeId(type_id));
    }
    pool.boot();
    pool.join();

    char buffer[9];
    std::string group_info_str;
    for (auto &&tmp : all_cases) {
        sprintf(buffer, "%d,%d\n", tmp.group_id, tmp.group_index);
        group_info_str += buffer;
    }
    auto group_info_md5 = md5(group_info_str.c_str(), group_info_str.size());
    EXPECT_STREQ(group_info_md5.c_str(), GROUP_INFO_MD5); // verify all group info
}

// TODO: verify GROUP_SEEDS_INDEX_REV
