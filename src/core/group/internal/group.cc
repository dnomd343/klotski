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
KLSK_NOINLINE static void extend(RawCode seed, const size_t size, MF add_mirror, RF release) {
    std::vector<RawCode> queue, mirrors;
    phmap::flat_hash_map<RawCode, uint64_t> cases;

    queue.reserve(size); mirrors.reserve(size);
    cases.reserve(static_cast<size_t>(static_cast<double>(size) * 1.56)); // reduce load factor

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

static RangesUnion extend_type_common(RawCode seed, size_t reserve) {
    RangesUnion data {};
    extend(seed, reserve, [](RawCode, auto) {}, [&data](const RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    });
    return data;
}

static RangesUnion extend_type_hor(RawCode seed, size_t reserve) {

    // size_t size_a = static_cast<size_t>(static_cast<double>(size) * 0.500892) + 11;
    // size_t size_b = static_cast<size_t>(static_cast<double>(size) * 0.499108) + 10;

    RangesUnion data {};

    // TODO: how to reserve
    // size_t val = reserve / 8;
    //
    // data.ranges(0).reserve(val);
    // data.ranges(1).reserve(val);
    // data.ranges(2).reserve(val);
    //
    // data.ranges(4).reserve(val);
    // data.ranges(5).reserve(val);
    // data.ranges(6).reserve(val);
    //
    // data.ranges(8).reserve(val);
    // data.ranges(9).reserve(val);
    // data.ranges(10).reserve(val);
    //
    // data.ranges(12).reserve(val);
    // data.ranges(13).reserve(val);
    // data.ranges(14).reserve(val);

    extend(seed, reserve, [](const RawCode code, auto callback) {
        if (const auto mirror = code.to_horizontal_mirror(); mirror != code) {
            callback(mirror);
        }
    }, [&data](const RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    });

    return data;
}

static RangesUnion extend_type_ver(RawCode seed, size_t reserve) {
    RangesUnion data {};
    extend(seed, reserve, [](const RawCode code, auto callback) {
        callback(code.to_vertical_mirror());
    }, [&data](const RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    });
    return data;
}

static RangesUnion extend_type_diag(RawCode seed, size_t reserve) {
    RangesUnion data {};
    extend(seed, reserve, [](const RawCode code, auto callback) {
        callback(code.to_diagonal_mirror());
    }, [&data](const RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    });
    return data;
}

static RangesUnion extend_type_x(RawCode seed, size_t reserve) {
    RangesUnion data {};
    extend(seed, reserve, [](const RawCode code, auto callback) {
        const auto mirror_1 = code.to_vertical_mirror();
        callback(mirror_1);
        if (const auto mirror_2 = code.to_horizontal_mirror(); mirror_2 != code) {
            callback(mirror_2);
            callback(mirror_1.to_horizontal_mirror());
        }
    }, [&data](const RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        data.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    });
    return data;
}

template <typename MFunc, typename RFunc>
KLSK_NOINLINE static void spawn_pattern(RawCode seed, const size_t reserve, MFunc add_mirror, RFunc release) {

    std::vector<RawCode> codes;
    std::vector<RawCode> mirrors;
    phmap::flat_hash_map<RawCode, uint64_t> cases; // <code, hint>

    codes.reserve(reserve);
    mirrors.reserve(reserve); // TODO: cal max size-coff
    cases.reserve(static_cast<size_t>(reserve * 1.56));

    auto core = MaskMover([&codes, &cases, &mirrors, add_mirror](RawCode code, uint64_t hint) {
        if (const auto [iter, ret] = cases.try_emplace(code, hint); !ret) {
            iter->second |= hint; // update hint
            return;
        }
        codes.emplace_back(code);
        add_mirror(code, [&cases, &mirrors](RawCode mirror) {
            cases.emplace(mirror, 0);
            mirrors.emplace_back(mirror);
        });
    });

    uint64_t offset = 0;
    codes.emplace_back(seed);
    cases.emplace(seed, 0); // without hint
    add_mirror(seed, [&mirrors, &cases](RawCode mirror) {
        cases.emplace(mirror, 0);
        mirrors.emplace_back(mirror);
    });
    while (offset != codes.size()) {
        const auto curr = codes[offset++];
        core.next_cases(curr, cases.find(curr)->second);
    }

    for (const auto code : codes) {
        release(code);
    }
    for (const auto code : mirrors) {
        release(code);
    }
}

KLSK_NOINLINE static void spawn_full_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    });
}

KLSK_NOINLINE static void spawn_hor_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

        const auto code_ = raw_code.to_vertical_mirror().to_common_code().unwrap();
        output.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
    });
}

KLSK_NOINLINE static void spawn_ver_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

        const auto code_ = raw_code.to_horizontal_mirror().to_common_code().unwrap();
        output.ranges(code_ >> 32).emplace_back(static_cast<uint32_t>(code_));
    });
}

KLSK_NOINLINE static void spawn_ord_pattern(RawCode seed, const size_t reserve, RangesUnion &output) {
    spawn_pattern(seed, reserve, [](RawCode, auto) {}, [&output](RawCode raw_code) {
        const auto code = raw_code.to_common_code().unwrap();
        output.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));

        const auto code_1 = raw_code.to_vertical_mirror().to_common_code().unwrap();
        output.ranges(code_1 >> 32).emplace_back(static_cast<uint32_t>(code_1));

        const auto code_2 = raw_code.to_horizontal_mirror().to_common_code().unwrap();
        output.ranges(code_2 >> 32).emplace_back(static_cast<uint32_t>(code_2));

        const auto code_3 = raw_code.to_diagonal_mirror().to_common_code().unwrap();
        output.ranges(code_3 >> 32).emplace_back(static_cast<uint32_t>(code_3));
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

    auto seed = CommonCode::unsafe_create(PATTERN_DATA[flat_id()] >> 23).to_raw_code();
    if (toward_ == Toward::B) {
        seed = seed.to_horizontal_mirror();
    } else if (toward_ == Toward::C) {
        seed = seed.to_vertical_mirror();
    } else if (toward_ == Toward::D) {
        seed = seed.to_diagonal_mirror();
    }

    RangesUnion data;
    if (mirror_type() == MirrorType::Full) {
        data = extend_type_x(seed, size());
    } else if (mirror_type() == MirrorType::Horizontal) {
        data = extend_type_hor(seed, size());
    } else if (mirror_type() == MirrorType::Vertical) {
        data = extend_type_ver(seed, size());
    } else if (mirror_type() == MirrorType::Centro) {
        data = extend_type_diag(seed, size());
    } else {
        data = extend_type_common(seed, size());
    }

    for (int head = 0; head < 16; ++head) {
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
