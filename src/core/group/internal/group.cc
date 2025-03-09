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
using klotski::group::PATTERN_OFFSET;

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

template <Group::MirrorType TYPE>
static RangesUnion build_group(const RawCode seed, const size_t reserve) {
    RangesUnion data {};
    if constexpr(TYPE == Group::MirrorType::Full) {
        const auto mirror_func = [](const RawCode code, auto spawn) {
            const auto m_vrt = code.to_vertical_mirror();
            spawn(m_vrt);
            if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
                spawn(m_hor);
                spawn(m_vrt.to_horizontal_mirror());
            }
        };
        extend(seed, reserve, mirror_func, RELEASE_INTO(data));
    }

    if constexpr(TYPE == Group::MirrorType::Horizontal) {
        const auto mirror_func = [](const RawCode code, auto spawn) {
            if (const auto m_hor = code.to_horizontal_mirror(); m_hor != code) {
                spawn(m_hor);
            }
        };
        extend(seed, reserve, mirror_func, RELEASE_INTO(data));
    }

    if constexpr(TYPE == Group::MirrorType::Centro) {
        const auto mirror_func = [](const RawCode code, auto spawn) {
            spawn(code.to_diagonal_mirror());
        };
        extend(seed, reserve, mirror_func, RELEASE_INTO(data));
    }

    if constexpr(TYPE == Group::MirrorType::Vertical) {
        const auto mirror_func = [](const RawCode code, auto spawn) {
            spawn(code.to_vertical_mirror());
        };
        extend(seed, reserve, mirror_func, RELEASE_INTO(data));
    }

    if constexpr(TYPE == Group::MirrorType::Ordinary) {
        extend(seed, reserve, NO_MIRROR, RELEASE_INTO(data));
    }
    return data;
}

#define EMPLACE_INTO_IMPL(VAR, EXPR, OUT) \
    const auto VAR = (EXPR).to_common_code().unwrap(); \
    OUT.ranges(VAR >> 32).emplace_back(static_cast<uint32_t>(VAR))

#define EMPLACE_INTO(RU, EXPR) \
    EMPLACE_INTO_IMPL(KLSK_UNIQUE(tmp), EXPR, RU)

template <Group::MirrorType TYPE>
KLSK_NOINLINE static void spawn_pattern(RawCode seed, const size_t reserve, RangesUnion &data) {
    if constexpr(TYPE == Group::MirrorType::Full) {
        extend(seed, reserve, NO_MIRROR, [&data](const RawCode code) {
            EMPLACE_INTO(data, code);
        });
    }

    if constexpr(TYPE == Group::MirrorType::Horizontal) {
        extend(seed, reserve, NO_MIRROR, [&data](const RawCode code) {
            EMPLACE_INTO(data, code);
            EMPLACE_INTO(data, code.to_vertical_mirror());
        });
    }

    if constexpr(TYPE == Group::MirrorType::Vertical) {
        extend(seed, reserve, NO_MIRROR, [&data](const RawCode code) {
            EMPLACE_INTO(data, code);
            EMPLACE_INTO(data, code.to_horizontal_mirror());
        });
    }

    if constexpr(TYPE == Group::MirrorType::Ordinary) {
        extend(seed, reserve, NO_MIRROR, [&data](const RawCode code) {
            // const auto m_vrt = code.to_vertical_mirror();
            // EMPLACE_INTO(data, code);
            // EMPLACE_INTO(data, m_vrt);
            // EMPLACE_INTO(data, code.to_horizontal_mirror());
            // EMPLACE_INTO(data, m_vrt.to_horizontal_mirror());
            EMPLACE_INTO(data, code);
            EMPLACE_INTO(data, code.to_vertical_mirror());
            EMPLACE_INTO(data, code.to_horizontal_mirror());
            EMPLACE_INTO(data, code.to_diagonal_mirror());
        });
    }
}

KLSK_NOINLINE static RangesUnion first_x_group(const uint8_t type_id) {
    RangesUnion others {};
    const auto gu = GroupUnion::unsafe_create(type_id);
    for (uint32_t pattern_id = 1; pattern_id < gu.pattern_num(); ++pattern_id) {
        const auto flat_id = PATTERN_OFFSET[type_id] + pattern_id;
        const auto type = static_cast<Group::MirrorType>(PATTERN_DATA[flat_id] & 0b111);
        const auto seed = CommonCode::unsafe_create(PATTERN_DATA[flat_id] >> 23).to_raw_code();
        const auto size = (PATTERN_DATA[flat_id] >> 3) & 0xFFFFF;

        if (type == Group::MirrorType::Full) {
            spawn_pattern<Group::MirrorType::Full>(seed, size, others);
        } else if (type == Group::MirrorType::Horizontal) {
            spawn_pattern<Group::MirrorType::Horizontal>(seed, size, others);
        } else if (type == Group::MirrorType::Vertical) {
            spawn_pattern<Group::MirrorType::Vertical>(seed, size, others);
        } else if (type == Group::MirrorType::Ordinary) {
            spawn_pattern<Group::MirrorType::Ordinary>(seed, size, others);
        }
    }
    for (const auto head : RangesUnion::Heads) {
        std::stable_sort(others.ranges(head).begin(), others.ranges(head).end());
    }
    return GroupUnion::unsafe_create(type_id).cases_without(others);
}

RangesUnion Group::cases() const {
    if (GroupUnion::unsafe_create(type_id_).group_num() == 1) {
        return GroupUnion::unsafe_create(type_id_).cases();
    }

    // + 116:  3321us vs 3162us
    // - 149:   989us vs 1406us
    // - 154:  4257us vs 6521us
    // + 159: 12801us vs 11804us
    // - 164: 13067us vs 13974us
    // - 181:   206us vs 613us
    // - 184:   170us vs 935us
    // - 185:   733us vs 3674us
    // - 187:   391us vs 530us
    // - 188:   587us vs 3155us

    if (pattern_id_ == 0 && mirror_type() == MirrorType::Full) {

        // TODO: it seems that this filter has a negative impact on non-full group cases performance.
        // if (type_id_ != 149 && type_id_ != 154 && type_id_ != 164 && type_id_ != 181 && type_id_ != 184 && type_id_ != 185 && type_id_ != 187 && type_id_ != 188) {
        // if (type_id_ != 149 && type_id_ != 154 && type_id_ != 164 && type_id_ < 181) {
            return first_x_group(type_id_);
        // }

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
        data = build_group<MirrorType::Full>(seed, size());
    } else if (mirror_type() == MirrorType::Horizontal) {
        data = build_group<MirrorType::Horizontal>(seed, size());
    } else if (mirror_type() == MirrorType::Vertical) {
        data = build_group<MirrorType::Vertical>(seed, size());
    } else if (mirror_type() == MirrorType::Centro) {
        data = build_group<MirrorType::Centro>(seed, size());
    } else {
        data = build_group<MirrorType::Ordinary>(seed, size());
    }
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
