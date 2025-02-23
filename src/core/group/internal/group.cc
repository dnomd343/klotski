#include <iostream>
#include <algorithm>
#include <parallel_hashmap/phmap.h>

#include "group/group.h"
#include "mover/mover.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::group::Group;
using klotski::group::GroupUnion;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;

using klotski::group::GROUP_DATA;
using klotski::group::PATTERN_DATA;

#define RELEASE_TO(RU) \
    [&data](const RawCode raw_code) { \
        const auto code = raw_code.to_common_code().unwrap(); \
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code)); \
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

template <Group::MirrorType TYPE>
static RangesUnion spawn_group(const RawCode seed, const size_t reserve) {
    const auto ctr_mirror = [](const RawCode code, auto spawn) {
        spawn(code.to_diagonal_mirror());
    };

    const auto vrt_mirror = [](const RawCode code, auto spawn) {
        spawn(code.to_vertical_mirror());
    };

    const auto hor_mirror = [](const RawCode code, auto spawn) {
        if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
            spawn(m_hor);
        }
    };

    const auto full_mirror = [](const RawCode code, auto spawn) {
        const auto m_vrt = code.to_vertical_mirror();
        spawn(m_vrt);
        if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
            spawn(m_hor);
            spawn(m_vrt.to_horizontal_mirror());
        }
    };

    RangesUnion data {}; // TODO: how to reserve `data`

    // TODO: it seems that reserve will actually lose speed, why???
    // auto val = reserve / 8;
    // data.ranges(0).reserve(val); data.ranges(1).reserve(val); data.ranges(2).reserve(val);
    // data.ranges(4).reserve(val); data.ranges(5).reserve(val); data.ranges(6).reserve(val);
    // data.ranges(8).reserve(val); data.ranges(9).reserve(val); data.ranges(10).reserve(val);
    // data.ranges(12).reserve(val); data.ranges(13).reserve(val); data.ranges(14).reserve(val);

    // TODO: maybe we don't need `else`
    if constexpr(TYPE == Group::MirrorType::Full) {
        extend(seed, reserve, full_mirror, RELEASE_TO(data));
    } else if constexpr(TYPE == Group::MirrorType::Horizontal) {
        extend(seed, reserve, hor_mirror, RELEASE_TO(data));
    } else if constexpr(TYPE == Group::MirrorType::Centro) {
        extend(seed, reserve, ctr_mirror, RELEASE_TO(data));
    } else if constexpr(TYPE == Group::MirrorType::Vertical) {
        extend(seed, reserve, vrt_mirror, RELEASE_TO(data));
    } else if constexpr(TYPE == Group::MirrorType::Ordinary) {
        extend(seed, reserve, [](RawCode, auto) {}, RELEASE_TO(data));
    }
    return data;
}

// static RangesUnion extend_type_ord(const RawCode seed, const size_t reserve) {
//     RangesUnion data {};
//     extend(seed, reserve, [](RawCode, auto) {}, RELEASE_TO(data));
//     return data;
// }
//
// static RangesUnion extend_type_hor(const RawCode seed, const size_t reserve) {
//     RangesUnion data {};
//     extend(seed, reserve, [](const RawCode code, auto callback) {
//         if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
//             callback(m_hor);
//         }
//     }, RELEASE_TO(data));
//     return data;
// }
//
// static RangesUnion extend_type_vrt(const RawCode seed, const size_t reserve) {
//     RangesUnion data {};
//     extend(seed, reserve, [](const RawCode code, auto callback) {
//         callback(code.to_vertical_mirror());
//     }, RELEASE_TO(data));
//     return data;
// }
//
// static RangesUnion extend_type_diag(const RawCode seed, const size_t reserve) {
//     RangesUnion data {};
//     extend(seed, reserve, [](const RawCode code, auto callback) {
//         callback(code.to_diagonal_mirror());
//     }, RELEASE_TO(data));
//     return data;
// }
//
// static RangesUnion extend_type_full(const RawCode seed, const size_t reserve) {
//     RangesUnion data {};
//     extend(seed, reserve, [](const RawCode code, auto callback) {
//         const auto m_vrt = code.to_vertical_mirror();
//         callback(m_vrt);
//         if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
//             callback(m_hor);
//             callback(m_vrt.to_horizontal_mirror());
//         }
//     }, RELEASE_TO(data));
//     return data;
// }

// template <typename MF, typename RF>
// KLSK_NOINLINE static void extend_2(const RawCode seed, const size_t reserve, MF add_mirror, RF release) {
//     std::vector<RawCode> queue, mirrors;
//     phmap::flat_hash_map<RawCode, uint64_t> cases;
//
//     queue.reserve(reserve); mirrors.reserve(reserve);
//     cases.reserve(static_cast<size_t>(static_cast<double>(reserve) * 1.56)); // reduce load factor
//
//     auto mover = MaskMover([&queue, &cases, &mirrors, add_mirror](RawCode code, uint64_t hint) {
//         if (const auto [iter, ret] = cases.try_emplace(code, hint); !ret) {
//             iter->second |= hint; // update hint
//             return;
//         }
//         queue.emplace_back(code);
//         add_mirror(code, [&cases, &mirrors](RawCode mirror) {
//             cases.emplace(mirror, 0); // without hint
//             mirrors.emplace_back(mirror);
//         });
//     });
//
//     queue.emplace_back(seed);
//     cases.emplace(seed, 0);
//     add_mirror(seed, [&mirrors, &cases](RawCode mirror) {
//         cases.emplace(mirror, 0); // without hint
//         mirrors.emplace_back(mirror);
//     });
//
//     uint64_t offset = 0;
//     while (offset != queue.size()) {
//         const auto curr = queue[offset++];
//         mover.next_cases(curr, cases.find(curr)->second);
//     }
//
//     for (const auto code : queue) { release(code); }
//     for (const auto code : mirrors) { release(code); }
// }

#define OUTPUT_IMPL(VAR, EXPL, OUT) \
    const auto VAR = (EXPL).to_common_code().unwrap(); \
    OUT.ranges(VAR >> 32).emplace_back(static_cast<uint32_t>(VAR))

#define OUTPUT(EXPL, OUT) \
    OUTPUT_IMPL(KLSK_UNIQUE(tmp), EXPL, OUT)

template <Group::MirrorType TYPE>
KLSK_NOINLINE static void spawn_xxx_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {

    if constexpr(TYPE == Group::MirrorType::Full) {
        extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
            OUTPUT(raw_code, output);
        });
    }

    if constexpr(TYPE == Group::MirrorType::Horizontal) {
        extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
            OUTPUT(raw_code, output);
            OUTPUT(raw_code.to_vertical_mirror(), output);
        });
    }

    if constexpr(TYPE == Group::MirrorType::Vertical) {
        extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
            OUTPUT(raw_code, output);
            OUTPUT(raw_code.to_horizontal_mirror(), output);
        });
    }

    if constexpr(TYPE == Group::MirrorType::Ordinary) {
        extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
            OUTPUT(raw_code, output);
            OUTPUT(raw_code.to_vertical_mirror(), output);
            OUTPUT(raw_code.to_horizontal_mirror(), output);
            OUTPUT(raw_code.to_diagonal_mirror(), output); // TODO: perf it
        });
    }
}

// KLSK_NOINLINE static void spawn_full_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
//     extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
//         OUTPUT(raw_code, output);
//     });
// }
//
// KLSK_NOINLINE static void spawn_hor_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
//     extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
//         OUTPUT(raw_code, output);
//         OUTPUT(raw_code.to_vertical_mirror(), output);
//     });
// }
//
// KLSK_NOINLINE static void spawn_ver_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
//     extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
//         OUTPUT(raw_code, output);
//         OUTPUT(raw_code.to_horizontal_mirror(), output);
//     });
// }
//
// KLSK_NOINLINE static void spawn_ord_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
//     extend(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
//         OUTPUT(raw_code, output);
//         OUTPUT(raw_code.to_vertical_mirror(), output);
//         OUTPUT(raw_code.to_horizontal_mirror(), output);
//         OUTPUT(raw_code.to_diagonal_mirror(), output); // TODO: perf it
//     });
// }

KLSK_NOINLINE static RangesUnion extend_pro(uint8_t type_id) {
    RangesUnion others {};
    auto gu = GroupUnion::unsafe_create(type_id);
    for (int pattern_id = 1; pattern_id < gu.pattern_num(); ++pattern_id) {

        auto flat_id = klotski::group::PATTERN_OFFSET[type_id] + pattern_id;
        auto mirror_type = static_cast<Group::MirrorType>(PATTERN_DATA[flat_id] & 0b111);
        auto seed = CommonCode::unsafe_create(PATTERN_DATA[flat_id] >> 23).to_raw_code();
        auto size = (PATTERN_DATA[flat_id] >> 3) & 0xFFFFF;

        if (mirror_type == Group::MirrorType::Full) {
            spawn_xxx_pattern<Group::MirrorType::Full>(seed, size, others);
            // spawn_full_pattern(seed, size, others);
        } else if (mirror_type == Group::MirrorType::Horizontal) {
            // spawn_hor_pattern(seed, size, others);
            spawn_xxx_pattern<Group::MirrorType::Horizontal>(seed, size, others);
        } else if (mirror_type == Group::MirrorType::Vertical) {
            // spawn_ver_pattern(seed, size, others);
            spawn_xxx_pattern<Group::MirrorType::Vertical>(seed, size, others);
        } else if (mirror_type == Group::MirrorType::Ordinary) {
            // spawn_ord_pattern(seed, size, others);
            spawn_xxx_pattern<Group::MirrorType::Ordinary>(seed, size, others);
        }
    }

    for (auto head : RangesUnion::Heads) {
        std::stable_sort(others.ranges(head).begin(), others.ranges(head).end());
    }
    // return others;

    return GroupUnion::unsafe_create(type_id).cases_without(others);
}

RangesUnion Group::cases() const {
    if (const auto gu = GroupUnion::unsafe_create(type_id_); gu.group_num() == 1) {
        // std::println("type_a: {}", size());
        return gu.cases();
    }

    if (pattern_id_ == 0 && mirror_type() == MirrorType::Full) { // TODO: black-list filter
        // std::println("type_b: {}", size());
        return extend_pro(type_id_);
        // return GroupUnion::unsafe_create(type_id_).cases();
    }

    const auto seed_val = PATTERN_DATA[flat_id()] >> 23;
    auto seed = CommonCode::unsafe_create(seed_val).to_raw_code();
    if (toward_ == Toward::B) {
        seed = seed.to_horizontal_mirror();
    } else if (toward_ == Toward::C) {
        seed = seed.to_vertical_mirror();
    } else if (toward_ == Toward::D) {
        seed = seed.to_diagonal_mirror();
    }

    RangesUnion data;
    if (mirror_type() == MirrorType::Full) {
        data = spawn_group<MirrorType::Full>(seed, size());
    } else if (mirror_type() == MirrorType::Horizontal) {
        data = spawn_group<MirrorType::Horizontal>(seed, size());
    } else if (mirror_type() == MirrorType::Vertical) {
        data = spawn_group<MirrorType::Vertical>(seed, size());
    } else if (mirror_type() == MirrorType::Centro) {
        data = spawn_group<MirrorType::Centro>(seed, size());
    } else {
        data = spawn_group<MirrorType::Ordinary>(seed, size());
    }
    for (const auto head : RangesUnion::Heads) {
        std::stable_sort(data.ranges(head).begin(), data.ranges(head).end());
    }
    // std::println("type_c: {}", size());
    return data;
}

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

static std::vector<RawCode> Group_extend_for_from_raw_code(RawCode raw_code) {
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

// TODO: maybe we can search new cases in `map_data`
Group Group::from_raw_code(const RawCode raw_code) {
    static auto map_data = build_map_data();

    auto raw_codes = Group_extend_for_from_raw_code(raw_code);
    auto common_codes = raw_codes | std::views::transform([](const RawCode r) {
        return r.to_common_code();
    }) | std::ranges::to<std::vector>(); // TODO: search min_element directly
    auto seed = std::min_element(common_codes.begin(), common_codes.end());

    return map_data.at(seed->unwrap());
}
