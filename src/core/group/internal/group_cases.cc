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

struct case_info_t {
    uint32_t pattern_id : 10;
    uint32_t toward_id : 2;
    uint32_t case_id  : 20;
};

static_assert(sizeof(case_info_t) == 4);

static std::array<RangesUnion, ALL_PATTERN_NUM * 4> *ru_data_array = nullptr;

static std::vector<case_info_t> *rev_data = nullptr;

#define RELEASE_INTO(RU) \
    [&RU](const RawCode raw_code) { \
        const auto code = raw_code.to_common_code().unwrap(); \
        RU.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code)); \
    }

#define NO_MIRROR \
    [](RawCode, auto) {}

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

// KLSK_NOINLINE static void extend_full_pattern(RawCode seed, size_t size, RangesUnion &a) {
//     const auto mirror_func = [](const RawCode code, auto spawn) {
//         const auto m_vrt = code.to_vertical_mirror();
//         spawn(m_vrt);
//         if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
//             spawn(m_hor);
//             spawn(m_vrt.to_horizontal_mirror());
//         }
//     };
//
//     extend(seed, size, mirror_func, [&a](const RawCode raw_code) {
//         const auto code = raw_code.to_common_code().unwrap();
//         a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
//     });
//
//     for (auto head : RangesUnion::Heads) {
//         std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
//     }
//
// }

// KLSK_NOINLINE static void extend_hor_pattern(RawCode seed, size_t size, RangesUnion &a, RangesUnion &c) {
//     const auto mirror_func = [](const RawCode code, auto spawn) {
//         if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
//             spawn(m_hor);
//         }
//     };
//     extend(seed, size, mirror_func, [&a, &c](const RawCode raw_code) {
//         const auto code = raw_code.to_common_code().unwrap();
//         a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
//
//         const auto code_ = raw_code.to_vertical_mirror().to_common_code().unwrap();
//         c.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
//     });
//
//     for (auto head : RangesUnion::Heads) {
//         std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
//         std::stable_sort(c.ranges(head).begin(), c.ranges(head).end());
//     }
//
// }

// KLSK_NOINLINE static void extend_cen_pattern(RawCode seed, size_t size, RangesUnion &a, RangesUnion &b) {
//     const auto mirror_func = [](const RawCode code, auto spawn) {
//         spawn(code.to_diagonal_mirror());
//     };
//     extend(seed, size, mirror_func, [&a, &b](const RawCode raw_code) {
//         const auto code = raw_code.to_common_code().unwrap();
//         a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
//
//         const auto code_ = raw_code.to_horizontal_mirror().to_common_code().unwrap();
//         b.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
//     });
//
//     for (auto head : RangesUnion::Heads) {
//         std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
//         std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
//     }
// }

// KLSK_NOINLINE static void extend_ver_pattern(RawCode seed, size_t size, RangesUnion &a, RangesUnion &b) {
//     const auto mirror_func = [](const RawCode code, auto spawn) {
//         spawn(code.to_vertical_mirror());
//     };
//     extend(seed, size, mirror_func, [&a, &b](const RawCode raw_code) {
//         const auto code = raw_code.to_common_code().unwrap();
//         a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
//
//         const auto code_ = raw_code.to_horizontal_mirror().to_common_code().unwrap();
//         b.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
//     });
//
//     for (auto head : RangesUnion::Heads) {
//         std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
//         std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
//     }
// }

// KLSK_NOINLINE static void extend_ord_pattern(RawCode seed, size_t size, RangesUnion &a, RangesUnion &b, RangesUnion &c, RangesUnion &d) {
//     extend(seed, size, NO_MIRROR, [&a, &b, &c, &d](const RawCode raw_code) {
//         const auto code = raw_code.to_common_code().unwrap();
//         a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
//
//         const auto code_1 = raw_code.to_horizontal_mirror().to_common_code().unwrap();
//         b.ranges(code_1 >> 32).emplace_back(static_cast<uint32_t>(code_1));
//
//         const auto code_2 = raw_code.to_vertical_mirror().to_common_code().unwrap();
//         c.ranges(code_2 >> 32).emplace_back(static_cast<uint32_t>(code_2));
//
//         const auto code_3 = raw_code.to_diagonal_mirror().to_common_code().unwrap();
//         d.ranges(code_3 >> 32).emplace_back(static_cast<uint32_t>(code_3));
//     });
//
//     for (auto head : RangesUnion::Heads) {
//         std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
//         std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
//         std::stable_sort(c.ranges(head).begin(), c.ranges(head).end());
//         std::stable_sort(d.ranges(head).begin(), d.ranges(head).end());
//     }
// }

template <Group::MirrorType TYPE>
// static void extend_pattern(RawCode seed, size_t size, RangesUnion &a, RangesUnion &b, RangesUnion &c, RangesUnion &d) {
static void extend_pattern(RawCode seed, size_t size, RangesUnion *kk) {

    RangesUnion &a = kk[0];
    RangesUnion &b = kk[1];
    RangesUnion &c = kk[2];
    RangesUnion &d = kk[3];

    if constexpr(TYPE == Group::MirrorType::Full) {

        const auto mirror_func = [](const RawCode code, auto spawn) {
            const auto m_vrt = code.to_vertical_mirror();
            spawn(m_vrt);
            if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
                spawn(m_hor);
                spawn(m_vrt.to_horizontal_mirror());
            }
        };

        // RangesUnion &a = kk[0];
        extend(seed, size, mirror_func, [&a](const RawCode raw_code) {
            const auto code = raw_code.to_common_code().unwrap();
            a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
        });

        // for (auto head : RangesUnion::Heads) {
        //     std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
        // }

    }

    if constexpr(TYPE == Group::MirrorType::Horizontal) {

        const auto mirror_func = [](const RawCode code, auto spawn) {
            if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
                spawn(m_hor);
            }
        };

        // RangesUnion &a = kk[0];
        // RangesUnion &c = kk[2];
        extend(seed, size, mirror_func, [&a, &c](const RawCode raw_code) {
            const auto code = raw_code.to_common_code().unwrap();
            a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

            const auto code_ = raw_code.to_vertical_mirror().to_common_code().unwrap();
            c.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
        });

        // for (auto head : RangesUnion::Heads) {
        //     std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
        //     std::stable_sort(c.ranges(head).begin(), c.ranges(head).end());
        // }

    }

    if constexpr(TYPE == Group::MirrorType::Centro) {

        const auto mirror_func = [](const RawCode code, auto spawn) {
            spawn(code.to_diagonal_mirror());
        };
        // RangesUnion &a = kk[0];
        // RangesUnion &b = kk[1];
        extend(seed, size, mirror_func, [&a, &b](const RawCode raw_code) {
            const auto code = raw_code.to_common_code().unwrap();
            a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

            const auto code_ = raw_code.to_horizontal_mirror().to_common_code().unwrap();
            b.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
        });

        // for (auto head : RangesUnion::Heads) {
        //     std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
        //     std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
        // }

    }

    if constexpr(TYPE == Group::MirrorType::Vertical) {

        const auto mirror_func = [](const RawCode code, auto spawn) {
            spawn(code.to_vertical_mirror());
        };
        // RangesUnion &a = kk[0];
        // RangesUnion &b = kk[1];
        extend(seed, size, mirror_func, [&a, &b](const RawCode raw_code) {
            const auto code = raw_code.to_common_code().unwrap();
            a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

            const auto code_ = raw_code.to_horizontal_mirror().to_common_code().unwrap();
            b.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
        });

        // for (auto head : RangesUnion::Heads) {
        //     std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
        //     std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
        // }

    }

    if constexpr(TYPE == Group::MirrorType::Ordinary) {

        // RangesUnion &a = kk[0];
        // RangesUnion &b = kk[1];
        // RangesUnion &c = kk[2];
        // RangesUnion &d = kk[3];
        extend(seed, size, NO_MIRROR, [&a, &b, &c, &d](const RawCode raw_code) {
            const auto code = raw_code.to_common_code().unwrap();
            a.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

            const auto code_1 = raw_code.to_horizontal_mirror().to_common_code().unwrap();
            b.ranges(code_1 >> 32).emplace_back(static_cast<uint32_t>(code_1));

            const auto code_2 = raw_code.to_vertical_mirror().to_common_code().unwrap();
            c.ranges(code_2 >> 32).emplace_back(static_cast<uint32_t>(code_2));

            const auto code_3 = raw_code.to_diagonal_mirror().to_common_code().unwrap();
            d.ranges(code_3 >> 32).emplace_back(static_cast<uint32_t>(code_3));
        });

        // for (auto head : RangesUnion::Heads) {
        //     std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());
        //     std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
        //     std::stable_sort(c.ranges(head).begin(), c.ranges(head).end());
        //     std::stable_sort(d.ranges(head).begin(), d.ranges(head).end());
        // }

    }


    for (auto head : RangesUnion::Heads) {
        std::stable_sort(a.ranges(head).begin(), a.ranges(head).end());

        if constexpr(TYPE == Group::MirrorType::Centro || TYPE == Group::MirrorType::Vertical || TYPE == Group::MirrorType::Ordinary) {
            std::stable_sort(b.ranges(head).begin(), b.ranges(head).end());
        }

        if constexpr(TYPE == Group::MirrorType::Horizontal || TYPE == Group::MirrorType::Ordinary) {
            std::stable_sort(c.ranges(head).begin(), c.ranges(head).end());
        }

        if constexpr(TYPE == Group::MirrorType::Ordinary) {
            std::stable_sort(d.ranges(head).begin(), d.ranges(head).end());
        }
    }

}


static void build_ru_arr(uint8_t type_id, std::array<RangesUnion, ALL_PATTERN_NUM * 4> &data) {
    auto group_union = GroupUnion::unsafe_create(type_id);

    if (group_union.group_num() == 1) { // only single group
        data[PATTERN_OFFSET[type_id] * 4] = group_union.cases();
        return;
    }

    uint32_t pattern_id_begin = 0;
    if ((PATTERN_DATA[PATTERN_OFFSET[type_id]] & 0b111) == 0) { // first pattern is `x`
        pattern_id_begin = 1;
    }

    for (uint32_t pattern_id = pattern_id_begin; pattern_id < group_union.pattern_num(); ++pattern_id) {

        const auto flat_id = PATTERN_OFFSET[type_id] + pattern_id;
        const auto mirror_type = static_cast<Group::MirrorType>(PATTERN_DATA[flat_id] & 0b111);

        const auto seed_val = PATTERN_DATA[flat_id] >> 23;
        auto seed = CommonCode::unsafe_create(seed_val).to_raw_code();
        const auto size = (PATTERN_DATA[flat_id] >> 3) & 0xFFFFF;

        if (mirror_type == Group::MirrorType::Full) {
            // extend_full_pattern(seed, size, data[flat_id * 4]);
            // extend_pattern<Group::MirrorType::Full>(seed, size, data[flat_id * 4], data[flat_id * 4 + 1], data[flat_id * 4 + 2], data[flat_id * 4 + 3]);
            extend_pattern<Group::MirrorType::Full>(seed, size, &data[flat_id * 4]);

        } else if (mirror_type == Group::MirrorType::Horizontal) {
            // extend_hor_pattern(seed, size, data[flat_id * 4], data[flat_id * 4 + 2]);
            // extend_pattern<Group::MirrorType::Horizontal>(seed, size, data[flat_id * 4], data[flat_id * 4 + 1], data[flat_id * 4 + 2], data[flat_id * 4 + 3]);
            extend_pattern<Group::MirrorType::Horizontal>(seed, size, &data[flat_id * 4]);

        } else if (mirror_type == Group::MirrorType::Centro) {
            // extend_cen_pattern(seed, size, data[flat_id * 4], data[flat_id * 4 + 1]);
            // extend_pattern<Group::MirrorType::Centro>(seed, size, data[flat_id * 4], data[flat_id * 4 + 1], data[flat_id * 4 + 2], data[flat_id * 4 + 3]);
            extend_pattern<Group::MirrorType::Centro>(seed, size, &data[flat_id * 4]);

        } else if (mirror_type == Group::MirrorType::Vertical) {
            // extend_ver_pattern(seed, size, data[flat_id * 4], data[flat_id * 4 + 1]);
            // extend_pattern<Group::MirrorType::Vertical>(seed, size, data[flat_id * 4], data[flat_id * 4 + 1], data[flat_id * 4 + 2], data[flat_id * 4 + 3]);
            extend_pattern<Group::MirrorType::Vertical>(seed, size, &data[flat_id * 4]);

        } else if (mirror_type == Group::MirrorType::Ordinary) {
            // extend_ord_pattern(seed, size, data[flat_id * 4], data[flat_id * 4 + 1], data[flat_id * 4 + 2], data[flat_id * 4 + 3]);
            // extend_pattern<Group::MirrorType::Ordinary>(seed, size, data[flat_id * 4], data[flat_id * 4 + 1], data[flat_id * 4 + 2], data[flat_id * 4 + 3]);
            extend_pattern<Group::MirrorType::Ordinary>(seed, size, &data[flat_id * 4]);

        }

    }

    if (pattern_id_begin == 1) { // first pattern is `x`
        RangesUnion others;

        size_t index_begin = (PATTERN_OFFSET[type_id] + 1) * 4;
        size_t index_end = (PATTERN_OFFSET[type_id] + group_union.pattern_num()) * 4;

        for (size_t index = index_begin; index < index_end; ++index) {
            others += data[index];
        }

        for (auto head : RangesUnion::Heads) {
            std::stable_sort(others.ranges(head).begin(), others.ranges(head).end());
        }

        data[PATTERN_OFFSET[type_id] * 4] = group_union.cases_without(others);
    }
}

std::array<RangesUnion, ALL_PATTERN_NUM * 4> build_ru_array() {
    std::array<RangesUnion, ALL_PATTERN_NUM * 4> data;

    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {
        build_ru_arr(type_id, data);
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
