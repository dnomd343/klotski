#include <cstdint>
#include <iostream>

#include <algorithm>

#include <parallel_hashmap/phmap.h>

#include "group/group.h"
#include "mover/mover.h"

using klotski::codec::RawCode;
using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::group::Group;
using klotski::group::GroupCases;
using klotski::group::GroupUnion;

using klotski::cases::Ranges;
using klotski::cases::RangesUnion;

using klotski::group::CaseInfo;

using klotski::mover::MaskMover;

using klotski::group::ALL_GROUP_NUM;
using klotski::group::TYPE_ID_LIMIT;
using klotski::cases::ALL_CASES_NUM_;

using klotski::group::GROUP_DATA;

using klotski::group::PATTERN_DATA;
using klotski::group::PATTERN_OFFSET;
using klotski::group::ALL_PATTERN_NUM;

using klotski::group::BLOCK_NUM;
using klotski::cases::BASIC_RANGES_NUM;
using klotski::group::GROUP_UNION_CASES_NUM;

#define RANGE_RESERVE(HEAD, SIZE) cases.ranges(HEAD).reserve(SIZE)

#define RANGE_DERIVE_WITHOUT(HEAD) ranges.derive_without(HEAD, cases.ranges(HEAD), data.ranges(HEAD))

using MirrorType = Group::MirrorType;

struct case_info_t {
    uint32_t pattern_id : 10;
    uint32_t toward_id : 2;
    uint32_t case_id  : 20;
};

static_assert(sizeof(case_info_t) == 4);

static std::array<RangesUnion, ALL_PATTERN_NUM * 4> *ru_data_array = nullptr;

static std::vector<case_info_t> *rev_data = nullptr;

#define EMPLACE_INTO_IMPL(RU, VAR, EXPR) \
    const auto VAR = (EXPR).to_common_code().unwrap(); \
    RU.ranges(VAR >> 32).emplace_back(static_cast<uint32_t>(VAR));

#define EMPLACE_INTO(RU, EXPR) \
    EMPLACE_INTO_IMPL(RU, KLSK_UNIQUE(tmp), EXPR)

#define NO_MIRROR [](RawCode, auto) {}

#define CTR_MIRROR \
    [](const RawCode code, auto spawn) { \
        spawn(code.to_diagonal_mirror()); \
    }

#define HOR_MIRROR \
    [](const RawCode code, auto spawn) { \
        if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) { \
            spawn(m_hor); \
        } \
    }

#define VRT_MIRROR \
    [](const RawCode code, auto spawn) { \
        spawn(code.to_vertical_mirror()); \
    }

#define FULL_MIRROR \
    [](const RawCode code, auto spawn) { \
        const auto m_vrt = code.to_vertical_mirror(); spawn(m_vrt); \
        if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) { \
            spawn(m_hor); spawn(m_vrt.to_horizontal_mirror()); \
        } \
    }

template <typename MF, typename RF>
KLSK_NOINLINE static void extend(const RawCode seed, const size_t reserve, MF add_mirror, RF release) {
    std::vector<RawCode> queue, mirrors;
    phmap::flat_hash_map<RawCode, uint64_t> cases;

    queue.reserve(reserve); mirrors.reserve(reserve);
    cases.reserve(static_cast<size_t>(static_cast<double>(reserve) * 1.56)); // reduce load factor

    auto mover = MaskMover([&queue, &cases, &mirrors, add_mirror](RawCode code, uint64_t hint) {
        if (const auto [iter, ret] = cases.try_emplace(code, hint); !ret) {
            iter->second |= hint; // update hint
            return;
        }
        queue.emplace_back(code);
        add_mirror(code, [&cases, &mirrors](RawCode mirror) {
            cases.emplace(mirror, 0); // without hint
            mirrors.emplace_back(mirror);
        });
    });

    queue.emplace_back(seed);
    cases.emplace(seed, 0);
    add_mirror(seed, [&mirrors, &cases](RawCode mirror) {
        cases.emplace(mirror, 0); // without hint
        mirrors.emplace_back(mirror);
    });

    size_t offset = 0;
    while (offset != queue.size()) {
        const auto curr = queue[offset++];
        mover.next_cases(curr, cases.find(curr)->second);
    }
    for (const auto code : queue) { release(code); }
    for (const auto code : mirrors) { release(code); }
}

// #define RELEASE_TO(A, B, C, D) \
//     [&data_a, &data_b, &data_c, &data_d](const RawCode code) { \
//         if constexpr(A == 'A') { EMPLACE_INTO(data_a, code); } \
//         if constexpr(B == 'B') { EMPLACE_INTO(data_b, code.to_horizontal_mirror()); } \
//         if constexpr(C == 'C') { EMPLACE_INTO(data_c, code.to_vertical_mirror()); } \
//         if constexpr(D == 'D') { EMPLACE_INTO(data_d, code.to_diagonal_mirror()); } \
//     }

template <MirrorType TYPE>
static void spawn_pattern(RawCode seed, size_t size, RangesUnion *data) {
    RangesUnion &ga = data[0];
    RangesUnion &gb = data[1];
    RangesUnion &gc = data[2];
    RangesUnion &gd = data[3];

    if constexpr(TYPE == MirrorType::Full) {
        extend(seed, size, FULL_MIRROR, [&ga](const RawCode code) {
            EMPLACE_INTO(ga, code);
        });
        // extend(seed, size, HOR_MIRROR, RELEASE_TO('A', 0, 0, 0));
    }

    if constexpr(TYPE == MirrorType::Horizontal) {
        extend(seed, size, HOR_MIRROR, [&ga, &gc](const RawCode code) {
            EMPLACE_INTO(ga, code);
            EMPLACE_INTO(gc, code.to_vertical_mirror());
        });
        // extend(seed, size, HOR_MIRROR, RELEASE_TO('A', 0, 'C', 0));
    }

    if constexpr(TYPE == MirrorType::Centro) {
        extend(seed, size, CTR_MIRROR, [&ga, &gb](const RawCode code) {
            EMPLACE_INTO(ga, code);
            EMPLACE_INTO(gb, code.to_horizontal_mirror());
        });
        // extend(seed, size, CTR_MIRROR, RELEASE_TO('A', 'B', 0, 0));
    }

    if constexpr(TYPE == MirrorType::Vertical) {
        extend(seed, size, VRT_MIRROR, [&ga, &gb](const RawCode code) {
            EMPLACE_INTO(ga, code);
            EMPLACE_INTO(gb, code.to_horizontal_mirror());
        });
        // extend(seed, size, VRT_MIRROR, RELEASE_TO('A', 'B', 0, 0));
    }

    if constexpr(TYPE == MirrorType::Ordinary) {
        extend(seed, size, NO_MIRROR, [&ga, &gb, &gc, &gd](const RawCode code) {
            EMPLACE_INTO(ga, code);
            EMPLACE_INTO(gb, code.to_horizontal_mirror());
            EMPLACE_INTO(gc, code.to_vertical_mirror());
            EMPLACE_INTO(gd, code.to_diagonal_mirror());
        });
        // extend(seed, size, NO_MIRROR, RELEASE_TO('A', 'B', 'C', 'D'));
    }

    for (const auto head : RangesUnion::Heads) {
        std::stable_sort(ga.ranges(head).begin(), ga.ranges(head).end());
        if constexpr(TYPE == MirrorType::Centro || TYPE == MirrorType::Vertical || TYPE == MirrorType::Ordinary) {
            std::stable_sort(gb.ranges(head).begin(), gb.ranges(head).end());
        }
        if constexpr(TYPE == MirrorType::Horizontal || TYPE == MirrorType::Ordinary) {
            std::stable_sort(gc.ranges(head).begin(), gc.ranges(head).end());
        }
        if constexpr(TYPE == MirrorType::Ordinary) {
            std::stable_sort(gd.ranges(head).begin(), gd.ranges(head).end());
        }
    }
}

// KLSK_INLINE static std::tuple<MirrorType, RawCode, size_t> get_info(uint8_t type_id, uint16_t pattern_id) {
//     const auto flat_id = PATTERN_OFFSET[type_id] + pattern_id;

//     const auto mirror_type = static_cast<MirrorType>(PATTERN_DATA[flat_id] & 0b111);

//     const auto seed_val = PATTERN_DATA[flat_id] >> 23;
//     auto seed = CommonCode::unsafe_create(seed_val).to_raw_code();
//     const auto size = (PATTERN_DATA[flat_id] >> 3) & 0xFFFFF;

//     return {mirror_type, seed, size};
// }

static RangesUnion cases_without(uint8_t type_id, const RangesUnion &data) {
    Ranges ranges {};
    ranges.reserve(BASIC_RANGES_NUM[type_id]);
    const auto [n, n_2x1, n_1x1] = BLOCK_NUM[type_id];
    ranges.spawn(n, n_2x1, n_1x1);
    ranges.reverse();

    RangesUnion cases;
    const auto [na, nb, nc, nd] = GROUP_UNION_CASES_NUM[type_id];
    RANGE_RESERVE(0x0, na); RANGE_RESERVE(0x1, nb); RANGE_RESERVE(0x2, na);
    RANGE_RESERVE(0x4, nc); RANGE_RESERVE(0x5, nd); RANGE_RESERVE(0x6, nc);
    RANGE_RESERVE(0x8, nc); RANGE_RESERVE(0x9, nd); RANGE_RESERVE(0xA, nc);
    RANGE_RESERVE(0xC, na); RANGE_RESERVE(0xD, nb); RANGE_RESERVE(0xE, na);

    RANGE_DERIVE_WITHOUT(0x0); RANGE_DERIVE_WITHOUT(0x1); RANGE_DERIVE_WITHOUT(0x2);
    RANGE_DERIVE_WITHOUT(0x4); RANGE_DERIVE_WITHOUT(0x5); RANGE_DERIVE_WITHOUT(0x6);
    RANGE_DERIVE_WITHOUT(0x8); RANGE_DERIVE_WITHOUT(0x9); RANGE_DERIVE_WITHOUT(0xA);
    RANGE_DERIVE_WITHOUT(0xC); RANGE_DERIVE_WITHOUT(0xD); RANGE_DERIVE_WITHOUT(0xE);
    return cases;
}

static void build_ru_arr(uint8_t type_id, RangesUnion *output) {
    const auto gu = GroupUnion::unsafe_create(type_id);
    if (gu.group_num() == 1) { // with single group
        *output = gu.cases();
        return;
    }

    const bool ff_mode = (PATTERN_DATA[PATTERN_OFFSET[type_id]] & 0b111) == 0;

    // uint32_t pattern_id_begin = 0;
    // if ((PATTERN_DATA[PATTERN_OFFSET[type_id]] & 0b111) == 0) { // first pattern type `Full`
    //     pattern_id_begin = 1;
    // }

    for (uint16_t pattern_id = ff_mode ? 1 : 0; pattern_id < gu.pattern_num(); ++pattern_id) {

        const auto flat_id = PATTERN_OFFSET[type_id] + pattern_id;
        const auto mirror_type = static_cast<MirrorType>(PATTERN_DATA[flat_id] & 0b111);

        const auto seed_val = PATTERN_DATA[flat_id] >> 23;
        auto seed = CommonCode::unsafe_create(seed_val).to_raw_code();
        const auto size = (PATTERN_DATA[flat_id] >> 3) & 0xFFFFF;

        // const auto [mirror_type, seed, size] = get_info(type_id, pattern_id);

        const auto kk = output + pattern_id * 4;

        if (mirror_type == MirrorType::Full) {
            spawn_pattern<MirrorType::Full>(seed, size, kk);
        } else if (mirror_type == MirrorType::Horizontal) {
            spawn_pattern<MirrorType::Horizontal>(seed, size, kk);
        } else if (mirror_type == MirrorType::Centro) {
            spawn_pattern<MirrorType::Centro>(seed, size, kk);
        } else if (mirror_type == MirrorType::Vertical) {
            spawn_pattern<MirrorType::Vertical>(seed, size, kk);
        } else if (mirror_type == MirrorType::Ordinary) {
            spawn_pattern<MirrorType::Ordinary>(seed, size, kk);
        }
    }

    if (ff_mode) {
        RangesUnion others;

        // TODO: try to reserve
        for (auto *ptr = output + 4; ptr < output + gu.pattern_num() * 4; ++ptr) {
            others += *ptr;
        }

        for (const auto head : RangesUnion::Heads) {
            std::stable_sort(others.ranges(head).begin(), others.ranges(head).end());
        }
        // *output = gu.cases_without(others);
        *output = cases_without(type_id, others);
    }
}

std::array<RangesUnion, ALL_PATTERN_NUM * 4> build_ru_array() {
    std::array<RangesUnion, ALL_PATTERN_NUM * 4> data;

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        build_ru_arr(type_id, &data[PATTERN_OFFSET[type_id] * 4]);
    }

    /// verify
    // for (uint8_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
    //     const auto group_union = GroupUnion::unsafe_create(type_id);
    //     for (const auto group : group_union.groups()) {
    //         const auto flat_id = PATTERN_OFFSET[type_id] + group.pattern_id();
    //         const auto index = flat_id * 4 + (int)group.toward();
    //
    //         const auto a = data[index].codes();
    //         const auto b = group.cases().codes();
    //         if (a != b) {
    //             std::cout << "!!! error: " << group << " | " << a.size() << " vs " << b.size() << std::endl;
    //         }
    //     }
    // }

    return data;
}

static std::vector<case_info_t> build_tmp_data() {
    std::vector<case_info_t> data;
    data.resize(ALL_CASES_NUM_);
    ShortCode::speed_up(true);

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        auto group_union = GroupUnion::unsafe_create(type_id);
        for (auto group : group_union.groups()) {
            uint32_t pattern_id = group.pattern_id();
            auto toward_id = (uint32_t)group.toward();

            // TODO: batch mirror base on pattern
            auto codes = group.cases().codes();
            for (uint32_t case_id = 0; case_id < codes.size(); ++case_id) {
                auto short_code = codes[case_id].to_short_code();
                data[short_code.unwrap()] = case_info_t {
                    .pattern_id = pattern_id,
                    .toward_id = toward_id,
                    .case_id = case_id,
                };
            }
        }
    }
    return data;
}

void GroupCases::build() {
    // if (fast_) {
    //     return;
    // }
    // std::lock_guard guard {busy_};

    // TODO: make `data` as class member
    // static auto data_1 = build_ranges_unions();
    // static auto data_2 = build_tmp_data();
    // ru_data = &data_1;
    // rev_data = &data_2;

    // about 34.1ns
    static auto data_array = build_ru_array();
    ru_data_array = &data_array;

    // KLSK_MEM_BARRIER;
    // fast_ = true;
}

void GroupCases::build_async(Executor &&executor, Notifier &&callback) {
    // TODO: real multi-thread build
    executor([callback = std::move(callback)] {
        build();
        callback();
    });
}

CommonCode GroupCases::fast_obtain_code(CaseInfo info) {

    auto flat_id = PATTERN_OFFSET[info.group().type_id()] + info.group().pattern_id();

    // TODO: make offset table for perf
    auto &cases = (*ru_data_array)[flat_id * 4 + (int)info.group().toward()];

    uint64_t head = 0;

    auto case_id = info.case_id();
    for (;;) {
        if (case_id >= cases.ranges(head).size()) {
            case_id -= cases.ranges(head).size();
            ++head;
        } else {
            break;
        }
    }

    auto range = cases.ranges(head)[case_id];
    return CommonCode::unsafe_create(head << 32 | range);
}

CaseInfo GroupCases::fast_obtain_info(const ShortCode short_code) {
    uint16_t type_id = GroupUnion::from_short_code(short_code).unwrap(); // NOTE: need to convert as CommonCode
    uint16_t pattern_id = (*rev_data)[short_code.unwrap()].pattern_id;
    uint16_t toward_id = (*rev_data)[short_code.unwrap()].toward_id;
    auto case_id = (*rev_data)[short_code.unwrap()].case_id;

    auto group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward_id);
    return CaseInfo::unsafe_create(group, case_id);
}

CaseInfo GroupCases::fast_obtain_info(const CommonCode common_code) {
    auto short_code = common_code.to_short_code();
    uint16_t type_id = GroupUnion::from_common_code(common_code).unwrap();
    uint16_t pattern_id = (*rev_data)[short_code.unwrap()].pattern_id;
    uint16_t toward_id = (*rev_data)[short_code.unwrap()].toward_id;
    auto case_id = (*rev_data)[short_code.unwrap()].case_id;

    auto group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward_id);
    return CaseInfo::unsafe_create(group, case_id);
}

Group GroupCases::fast_obtain_group(const ShortCode short_code) {
    uint16_t type_id = GroupUnion::from_short_code(short_code).unwrap();
    uint16_t pattern_id = (*rev_data)[short_code.unwrap()].pattern_id;
    uint16_t toward_id = (*rev_data)[short_code.unwrap()].toward_id;
    return Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward_id);
}

Group GroupCases::fast_obtain_group(const CommonCode common_code) {
    auto short_code = common_code.to_short_code();
    uint16_t type_id = GroupUnion::from_common_code(common_code).unwrap();
    uint16_t pattern_id = (*rev_data)[short_code.unwrap()].pattern_id;
    uint16_t toward_id = (*rev_data)[short_code.unwrap()].toward_id;
    return Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward_id);
}

CommonCode GroupCases::tiny_obtain_code(CaseInfo info) {
    auto cases = info.group().cases();
    uint64_t head = 0;

    auto case_id = info.case_id();
    for (;;) {
        if (case_id >= cases.ranges(head).size()) {
            case_id -= cases.ranges(head).size();
            ++head;
        } else {
            break;
        }
    }

    auto range = cases.ranges(head)[case_id];
    return CommonCode::unsafe_create(head << 32 | range);
}

// NOTE: copy directly from Group impl
static std::unordered_map<uint64_t, Group> build_map_data() {
    // NOTE: using CommonCode as map key
    std::unordered_map<uint64_t, Group> data;
    data.reserve(GROUP_DATA.size());
    for (auto raw : GROUP_DATA) {
        uint32_t type_id = (raw >> 12) & 0b11111111;
        uint32_t pattern_id = (raw >> 2) & 0b1111111111;
        uint32_t toward = raw & 0b11;
        auto seed = CommonCode::unsafe_create(raw >> 20).unwrap();
        auto group = Group::unsafe_create(type_id, pattern_id, (Group::Toward)toward);
        data.emplace(seed, group);
    }
    return data;
}

static std::vector<RawCode> Group_extend_for_obtain_info(RawCode raw_code) {
    std::vector<RawCode> codes;
    phmap::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>

    codes.reserve(GroupUnion::from_raw_code(raw_code).max_group_size());
    cases.reserve(GroupUnion::from_raw_code(raw_code).max_group_size() * 1.56);

    auto core = MaskMover([&codes, &cases](RawCode code, uint64_t mask) {
        if (const auto match = cases.find(code.unwrap()); match != cases.end()) {
            match->second |= mask; // update mask
            return;
        }
        cases.emplace(code, mask);
        codes.emplace_back(code); // new case
    });

    uint64_t offset = 0;
    codes.emplace_back(raw_code);
    cases.emplace(raw_code, 0); // without mask
    while (offset != codes.size()) {
        auto curr = codes[offset++].unwrap();
        core.next_cases(RawCode::unsafe_create(curr), cases.find(curr)->second);
    }
    return codes;
}

CaseInfo GroupCases::tiny_obtain_info(const CommonCode common_code) {
    auto raw_codes = Group_extend_for_obtain_info(common_code.to_raw_code());
    std::vector<CommonCode> common_codes;
    common_codes.reserve(raw_codes.size());
    for (auto raw_code : raw_codes) {
        common_codes.emplace_back(raw_code.to_common_code());
    }

    static auto map_data = build_map_data(); // TODO: shared map data with Group::from_raw_code

    auto seed = std::min_element(common_codes.begin(), common_codes.end());
    auto group = map_data.at(seed->unwrap());

    // TODO: try to perf it
    std::sort(common_codes.begin(), common_codes.end());
    auto tmp = std::lower_bound(common_codes.begin(), common_codes.end(), common_code);
    auto case_id = tmp - common_codes.begin();

    return CaseInfo::unsafe_create(group, case_id);
}
