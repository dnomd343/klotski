#include <algorithm>
#include <thread>
#include <iostream>
#include <format>
#include <map>
#include <unordered_set>
#include <ranges/ranges.h>

#include "core/core.h"
#include "group/group.h"
#include "raw_code/raw_code.h"
#include "fast_cal/fast_cal.h"
#include "all_cases/all_cases.h"
#include "short_code/short_code.h"
#include "common_code/common_code.h"

#include "../../third_party/thread-pool/include/BS_thread_pool.hpp"

using klotski::core::Core;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;
using klotski::cases::GroupUnion;

using klotski::cases::Group;
using klotski::cases::GroupPro;
using klotski::cases::GroupCases;
using klotski::cases::GroupUnion;

using klotski::cases::TYPE_ID_LIMIT;
using klotski::codec::SHORT_CODE_LIMIT;

struct Pattern {
    enum class Mirror {
        Full,
        V,
        HV,
        H,
        Common,
    };

    Pattern(CommonCode s) : seed(s) {}

    Mirror mirror;
    uint32_t size;
    CommonCode seed;
    uint32_t group_size;
    std::array<std::vector<CommonCode>, 4> cases {};
};

std::vector<CommonCode> split_group(std::unordered_set<uint64_t> &codes, CommonCode origin) {
    std::vector<CommonCode> group;
    for (auto raw_code : Group::extend(origin.to_raw_code())) {
        auto common_code = raw_code.to_common_code();
        codes.erase(common_code.unwrap());
        group.emplace_back(common_code);
    }
    std::sort(group.begin(), group.end());
    return group;
}

std::vector<Pattern> split_patterns(const std::vector<CommonCode> &common_codes) {
    std::unordered_set<uint64_t> codes;
    for (auto code : common_codes) {
        codes.emplace(code.unwrap());
    }

    std::vector<Pattern> patterns;
    while (true) {
        if (codes.empty()) {
            break;
        }

        auto code_a = CommonCode::unsafe_create(*std::min_element(codes.begin(), codes.end()));
        auto code_b = code_a.to_horizontal_mirror();
        auto code_c = code_a.to_vertical_mirror();
        auto code_d = code_b.to_vertical_mirror();

        auto group_a = split_group(codes, code_a);
        auto group_b = split_group(codes, code_b);
        auto group_c = split_group(codes, code_c);
        auto group_d = split_group(codes, code_d);

        Pattern pattern {code_a};
        pattern.cases[0] = group_a;
        pattern.size = group_a.size();
        pattern.group_size = group_a.size();

        if (group_a.size() != group_b.size() || group_a.size() != group_c.size() || group_a.size() != group_d.size()) {
            std::cout << "group size not match" << std::endl;
            break;
        }

        if (group_a == group_b && group_a == group_c && group_a == group_d) {
            pattern.mirror = Pattern::Mirror::Full;
        } else if (group_a != group_b && group_a != group_c && group_a != group_d && \
                   group_b != group_c && group_b != group_d && group_c != group_d) {
            pattern.mirror = Pattern::Mirror::Common;
            pattern.cases[1] = group_b;
            pattern.cases[2] = group_c;
            pattern.cases[3] = group_d;
            pattern.size *= 4;
        } else if (group_a == group_b && group_c == group_d && group_a != group_c && group_b != group_d) {
            pattern.mirror = Pattern::Mirror::V;
            pattern.cases[1] = group_c;
            pattern.size *= 2;
        } else if (group_a == group_c && group_b == group_d && group_a != group_b && group_c != group_d) {
            pattern.mirror = Pattern::Mirror::H;
            pattern.cases[1] = group_b;
            pattern.size *= 2;
        } else if (group_a == group_d && group_b == group_c && group_a != group_b && group_c != group_d) {
            pattern.mirror = Pattern::Mirror::HV;
            pattern.cases[1] = group_b;
            pattern.size *= 2;
        } else {
            std::cout << "unknown pattern" << std::endl;
            break;
        }

        patterns.emplace_back(pattern);
    }

    std::sort(patterns.begin(), patterns.end(), [](const Pattern &lhs, const Pattern &rhs) {
        if (lhs.size > rhs.size) {
            return true;
        }
        if (lhs.size < rhs.size) {
            return false;
        }

        if ((int)lhs.mirror < (int)rhs.mirror) {
            return true;
        }
        if ((int)lhs.mirror > (int)rhs.mirror) {
            return false;
        }

        if (lhs.seed < rhs.seed) {
            return true;
        }
        if (lhs.seed > rhs.seed) {
            return false;
        }

        std::cout << "pattern compare error" << std::endl;
        return false;
    });
    return patterns;
}

int main() {
    // const auto start = clock();

    // auto ret = klotski::cases::Group::extend(RawCode::from_common_code(0xDAAFE0C00).value());
    // std::cout << ret.size() << std::endl;

    const auto start = std::chrono::system_clock::now();

//    const auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);
//    const auto group = Group::from_common_code(common_code);

//    GroupCases::build();
//
//    std::cout << GroupCases::fast_parse({169, 1, 7472}) << std::endl;
//    std::cout << GroupCases::fast_parse({164, 0, 30833}) << std::endl;
//
//    std::cout << GroupCases::fast_obtain(CommonCode::unsafe_create(0x1A9BF0C00)) << std::endl;
//    std::cout << GroupCases::fast_obtain(CommonCode::unsafe_create(0x4FEA13400)) << std::endl;

    uint32_t flat_id = 0;
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto gu = GroupUnion::unsafe_create(type_id);
        auto patterns = split_patterns(gu.cases().codes());
        if (gu.pattern_num() != patterns.size()) {
            std::cout << "pattern number error" << std::endl;
        }

        for (auto &pattern : patterns) {
            if (pattern.group_size != klotski::cases::GROUP_PRO_SIZE[flat_id]) {
                std::cout << "pattern group size error" << std::endl;
            }
            if ((int)pattern.mirror != klotski::cases::GROUP_PRO_TYPE[flat_id]) {
                std::cout << "pattern mirror type error" << std::endl;
            }
            if (pattern.seed != klotski::cases::GROUP_PRO_SEED[flat_id]) {
                std::cout << "pattern seed error" << std::endl;
            }
            ++flat_id;
        }
    }

//    std::cout << gu.pattern_num() << std::endl;
//    std::cout << gu.group_num() << std::endl;
//    std::cout << gu.groups_pro().size() << std::endl;
//
//    const auto g = gu.groups_pro()[1];
//    std::cout << std::format("{}-{}-{}\n", g.type_id(), g.pattern_id(), g.mirror_toward());
//
//    const auto codes = g.cases().codes();
//    std::cout << codes.size() << std::endl;
//    for (const auto code : codes) {
//        if (code == 0x1A9BF0C00) {
//            std::cout << "get it" << std::endl;
//        }
//    }
//
//    std::cout << g.is_vertical_mirror() << std::endl;
//    std::cout << g.is_horizontal_mirror() << std::endl;
//
//    auto g1 = g.to_vertical_mirror();
//    std::cout << std::format("{}-{}-{}\n", g1.type_id(), g1.pattern_id(), g1.mirror_toward());
//    auto g2 = g.to_horizontal_mirror();
//    std::cout << std::format("{}-{}-{}\n", g2.type_id(), g2.pattern_id(), g2.mirror_toward());

//    auto common_code = CommonCode::unsafe_create(0x1A9BF0C00);
//    auto common_code = CommonCode::unsafe_create(0x4FEA13400);
//    auto kk = GroupPro::from_common_code(common_code);
//    std::cout << std::format("{}-{}-{}\n", kk.type_id(), kk.pattern_id(), kk.mirror_toward());

//    auto gp = GroupPro::unsafe_create(169, 0, 0);
//    std::cout << gp.size() << std::endl;
//    std::cout << (int)gp.mirror_type() << std::endl;

    std::cerr << std::chrono::system_clock::now() - start << std::endl;

    // auto core = Core([](const uint64_t code, uint64_t) {
    //     std::cout << RawCode::unsafe_create(code);
    //     std::cout << std::endl;
    // });
    // core.next_cases(RawCode::from_common_code(0x1A9BF0C00).value().unwrap(), 0x0);

    // auto cal = FastCal(RawCode::from_common_code(0x1A9BF0C00).value());
    // auto cal = FastCal(RawCode::from_common_code("25EEF04").value());

    // auto ret = cal.solve();
    //
    // for (auto kk : cal.backtrack(ret)) {
    //     std::cout << kk.to_common_code() << "," << kk.to_common_code().to_short_code() << std::endl;
    // }

    // for (const auto solve : cal.solve_multi()) {
    //     for (const auto raw_code : cal.backtrack(solve)) {
    //         const auto common_code = raw_code.to_common_code();
    //         std::cout << common_code << "/" << common_code.to_short_code() << std::endl;
    //     }
    //     std::cout << "----" << std::endl;
    // }

    // std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

    return 0;
}
