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

#define RELEASE_TO(RU) \
    [&data](const RawCode raw_code) { \
        const auto code = raw_code.to_common_code().unwrap(); \
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code)); \
    }

template <Group::MirrorType TYPE>
static RangesUnion extend_type_xxx(const RawCode seed, const size_t reserve) {
    RangesUnion data {};

    if constexpr(TYPE == Group::MirrorType::Ordinary) {

        extend(seed, reserve, [](RawCode, auto) {}, RELEASE_TO(data));

    } else if constexpr(TYPE == Group::MirrorType::Horizontal) {

        extend(seed, reserve, [](const RawCode code, auto callback) {
            if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
                callback(m_hor);
            }
        }, RELEASE_TO(data));

    } else if constexpr(TYPE == Group::MirrorType::Vertical) {

        extend(seed, reserve, [](const RawCode code, auto callback) {
            callback(code.to_vertical_mirror());
        }, RELEASE_TO(data));

    } else if constexpr(TYPE == Group::MirrorType::Centro) {

        extend(seed, reserve, [](const RawCode code, auto callback) {
            callback(code.to_diagonal_mirror());
        }, RELEASE_TO(data));

    } else if constexpr(TYPE == Group::MirrorType::Full) {

        extend(seed, reserve, [](const RawCode code, auto callback) {
            const auto m_vrt = code.to_vertical_mirror();
            callback(m_vrt);
            if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
                callback(m_hor);
                callback(m_vrt.to_horizontal_mirror());
            }
        }, RELEASE_TO(data));

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

template <typename MFunc, typename RFunc>
KLSK_NOINLINE static void spawn_pattern(RawCode seed, const size_t reserve, MFunc add_mirror, RFunc release) {
    std::vector<RawCode> queue;
    std::vector<RawCode> mirrors;
    phmap::flat_hash_map<RawCode, uint64_t> cases;

    queue.reserve(reserve);
    mirrors.reserve(reserve);
    cases.reserve(static_cast<size_t>(reserve * 1.56));

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

    uint64_t offset = 0;
    queue.emplace_back(seed);
    cases.emplace(seed, 0);
    add_mirror(seed, [&mirrors, &cases](RawCode mirror) {
        cases.emplace(mirror, 0); // without hint
        mirrors.emplace_back(mirror);
    });
    while (offset != queue.size()) {
        const auto curr = queue[offset++];
        mover.next_cases(curr, cases.find(curr)->second);
    }

    for (const auto code : queue) { release(code); }
    for (const auto code : mirrors) { release(code); }
}

#define OUTPUT(VAR, EXPL, OUT) \
    const auto VAR = (EXPL).to_common_code().unwrap(); \
    OUT.ranges(VAR >> 32).emplace_back(static_cast<uint32_t>(VAR))

#define KLSK_CONCAT_IMPL(X, Y) X##Y

#define KLSK_CONCAT(X, Y) KLSK_CONCAT_IMPL(X, Y)

#define OUTPUT_IMPL(VAR, EXPL, OUT) \
    const auto VAR = (EXPL).to_common_code().unwrap(); \
    OUT.ranges(VAR >> 32).emplace_back(static_cast<uint32_t>(VAR))

#define OUTPUT(EXPL, OUT) \
    OUTPUT_IMPL(KLSK_CONCAT(c_, __COUNTER__), EXPL, OUT)

KLSK_NOINLINE static void spawn_full_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        // const auto code = raw_code.to_common_code().unwrap();
        // output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
        // OUTPUT(code, raw_code, output);
        OUTPUT(raw_code, output);
    });
}

KLSK_NOINLINE static void spawn_hor_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        // const auto code = raw_code.to_common_code().unwrap();
        // output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
        OUTPUT(raw_code, output);

        // const auto code_ = raw_code.to_vertical_mirror().to_common_code().unwrap();
        // output.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
        OUTPUT(raw_code.to_vertical_mirror(), output);
    });
}

KLSK_NOINLINE static void spawn_ver_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        // const auto code = raw_code.to_common_code().unwrap();
        // output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
        OUTPUT(raw_code, output);

        // const auto code_ = raw_code.to_horizontal_mirror().to_common_code().unwrap();
        // output.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
        OUTPUT(raw_code.to_horizontal_mirror(), output);
    });
}

KLSK_NOINLINE static void spawn_ord_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        // const auto code = raw_code.to_common_code().unwrap();
        // output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
        OUTPUT(raw_code, output);

        // const auto code_1 = raw_code.to_vertical_mirror().to_common_code().unwrap();
        // output.ranges(code_1 >> 32).emplace_back(static_cast<uint32_t>(code_1));
        OUTPUT(raw_code.to_vertical_mirror(), output);

        // const auto code_2 = raw_code.to_horizontal_mirror().to_common_code().unwrap();
        // output.ranges(code_2 >> 32).emplace_back(static_cast<uint32_t>(code_2));
        OUTPUT(raw_code.to_horizontal_mirror(), output);

        // const auto code_3 = raw_code.to_diagonal_mirror().to_common_code().unwrap();
        // output.ranges(code_3 >> 32).emplace_back(static_cast<uint32_t>(code_3));
        OUTPUT(raw_code.to_diagonal_mirror(), output); // TODO: perf it
    });
}

KLSK_NOINLINE static RangesUnion extend_pro(uint8_t type_id) {
    RangesUnion others {};
    auto gu = GroupUnion::unsafe_create(type_id);
    for (int pattern_id = 1; pattern_id < gu.pattern_num(); ++pattern_id) {

        auto flat_id = klotski::group::PATTERN_OFFSET[type_id] + pattern_id;
        auto mirror_type = static_cast<Group::MirrorType>(PATTERN_DATA[flat_id] & 0b111);
        auto seed = CommonCode::unsafe_create(PATTERN_DATA[flat_id] >> 23).to_raw_code();
        auto size = (PATTERN_DATA[flat_id] >> 3) & 0xFFFFF;

        if (mirror_type == Group::MirrorType::Full) {
            spawn_full_pattern(seed, size, others);
        } else if (mirror_type == Group::MirrorType::Horizontal) {
            spawn_hor_pattern(seed, size, others);
        } else if (mirror_type == Group::MirrorType::Vertical) {
            spawn_ver_pattern(seed, size, others);
        } else if (mirror_type == Group::MirrorType::Ordinary) {
            spawn_ord_pattern(seed, size, others);
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
        return gu.cases();
    }

    if (pattern_id_ == 0 && mirror_type() == MirrorType::Full) { // TODO: black-list filter
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

    // const auto centro_mirror = [](const RawCode code, auto callback) {
    //     callback(code.to_diagonal_mirror());
    // };
    // const auto vertical_mirror = [](const RawCode code, auto callback) {
    //     callback(code.to_vertical_mirror());
    // };
    // const auto horizontal_mirror = [](const RawCode code, auto callback) {
    //     if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
    //         callback(m_hor);
    //     }
    // };
    // const auto full_mirror = [](const RawCode code, auto callback) {
    //     const auto m_vrt = code.to_vertical_mirror();
    //     callback(m_vrt);
    //     if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
    //         callback(m_hor);
    //         callback(m_vrt.to_horizontal_mirror());
    //     }
    // };

    // TODO: how to reserve `data`
    RangesUnion data;

    // if (mirror_type() == MirrorType::Ordinary) {
    //     // std::println("Ordinary"); // 24892
    //     // extend(seed, size(), [](RawCode, auto) {}, RELEASE_TO(data));
    //
    //     data = extend_ord(seed, size());
    //
    // } else if (mirror_type() == MirrorType::Horizontal) {
    //     // std::println("Horizontal"); // 294
    //     // extend(seed, size(), [](const RawCode code, auto callback) {
    //     //     if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
    //     //         callback(m_hor);
    //     //     }
    //     // }, RELEASE_TO(data));
    //     // return extend_hor(seed, size());
    //
    //     data = extend_hor(seed, size());
    //
    // } else if (mirror_type() == MirrorType::Vertical) {
    //     // std::println("Vertical"); // 54
    //     // extend(seed, size(), vertical_mirror, RELEASE_TO(data));
    //
    //     data = extend_ver(seed, size());
    //
    // } else if (mirror_type() == MirrorType::Full) {
    //     // std::println("Full"); // 3
    //     // extend(seed, size(), full_mirror, RELEASE_TO(data));
    //
    //     data = extend_type_x(seed, size());
    //
    // } else {
    //     // std::println("Centro"); // 4
    //     // extend(seed, size(), centro_mirror, RELEASE_TO(data));
    //
    //     data = extend_diag(seed, size());
    //
    // }

    if (mirror_type() == MirrorType::Full) {
        // data = extend_type_full(seed, size());
        data = extend_type_xxx<MirrorType::Full>(seed, size());
    } else if (mirror_type() == MirrorType::Horizontal) {
        // data = extend_type_hor(seed, size());
        data = extend_type_xxx<MirrorType::Horizontal>(seed, size());
    } else if (mirror_type() == MirrorType::Vertical) {
        // data = extend_type_vrt(seed, size());
        data = extend_type_xxx<MirrorType::Vertical>(seed, size());
    } else if (mirror_type() == MirrorType::Centro) {
        // data = extend_type_diag(seed, size());
        data = extend_type_xxx<MirrorType::Centro>(seed, size());
    } else {
        // data = extend_type_ord(seed, size());
        data = extend_type_xxx<MirrorType::Ordinary>(seed, size());
    }

    // if (mirror_type() == MirrorType::Ordinary) {
    //     data = extend_type_ord(seed, size());
    // } else if (mirror_type() == MirrorType::Horizontal) {
    //     data = extend_type_hor(seed, size());
    // } else if (mirror_type() == MirrorType::Vertical) {
    //     data = extend_type_vrt(seed, size());
    // } else if (mirror_type() == MirrorType::Full) {
    //     data = extend_type_full(seed, size());
    // } else {
    //     data = extend_type_diag(seed, size());
    // }

    for (const auto head : RangesUnion::Heads) {
        std::stable_sort(data.ranges(head).begin(), data.ranges(head).end());
    }
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
