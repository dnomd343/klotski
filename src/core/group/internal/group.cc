#include <iostream>
#include <algorithm>

#include "group/group.h"

#include <mover/mover.h>

#include <parallel_hashmap/phmap.h>

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using klotski::group::Group;
using klotski::group::GroupUnion;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;

using klotski::group::GROUP_DATA;
using klotski::group::PATTERN_DATA;

// NOTE: This code implements two time-consuming functions:
//       1. Calculate all cases included in Group based on type_id/pattern_id/toward.
//       2. Calculate the Group information it belongs to based on RawCode.

/// Spawn all the unsorted codes of the current group.
// static std::vector<RawCode> Group_extend_for_cases(RawCode raw_code, uint32_t reserve) {
//     std::vector<RawCode> codes;
//     phmap::flat_hash_map<uint64_t, uint64_t> cases; // <code, mask>
//     codes.reserve(reserve);
//     cases.reserve(reserve * 1.56);
//
//     auto core = MaskMover([&codes, &cases](RawCode code, uint64_t mask) {
//         if (const auto match = cases.find(code.unwrap()); match != cases.end()) {
//             match->second |= mask; // update mask
//             return;
//         }
//         cases.emplace(code, mask);
//         codes.emplace_back(code); // new case
//     });
//
//     uint64_t offset = 0;
//     codes.emplace_back(raw_code);
//     cases.emplace(raw_code, 0); // without mask
//     while (offset != codes.size()) {
//         auto curr = codes[offset++].unwrap();
//         core.next_cases(RawCode::unsafe_create(curr), cases.find(curr)->second);
//     }
//     return codes;
// }

template <typename R>
static RangesUnion extend_next_(RawCode seed, size_t reserve, R func) {
    std::vector<RawCode> codes;
    std::vector<RawCode> mirrors;
    phmap::flat_hash_map<RawCode, uint64_t> cases; // <code, hint>

    codes.reserve(reserve);
    mirrors.reserve(reserve); // TODO: cal max size-coff
    cases.reserve(reserve * 1.56);

    auto core = MaskMover([&codes, &cases, &mirrors, func](RawCode code, uint64_t hint) {
        if (const auto [iter, ret] = cases.try_emplace(code, hint); !ret) {
            iter->second |= hint; // update hint
            return;
        }
        codes.emplace_back(code); // new case

        func(code, [&cases, &mirrors](RawCode kk) {
            cases.emplace(kk, 0); // TODO: contain check
            mirrors.emplace_back(kk);
        });
    });

    uint64_t offset = 0;
    codes.emplace_back(seed);
    cases.emplace(seed, 0); // without hint

    func(seed, [&mirrors, &cases](RawCode pp) {
        cases.emplace(pp, 0);
        mirrors.emplace_back(pp);
    });

    while (offset != codes.size()) {
        auto curr = codes[offset++];
        core.next_cases(curr, cases.find(curr)->second);
    }

    RangesUnion result {};
    // TODO: how to reserve
    for (auto raw_code : codes) { // TODO: using `std::views::concat` in new std library
        const auto code = raw_code.to_common_code().unwrap();
        result.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    }
    for (auto raw_code : mirrors) {
        const auto code = raw_code.to_common_code().unwrap();
        result.ranges(code >> 32).emplace_back(static_cast<uint32_t>(code));
    }
    return result;
}

__attribute__((noinline)) static RangesUnion extend_type_common(RawCode seed, size_t reserve) {
    return extend_next_(seed, reserve, [](RawCode code, auto callback) {});
}

// TODO: without `__attribute__((noinline))` will make it faster
static RangesUnion extend_type_hor(RawCode seed, size_t reserve) {
    return extend_next_(seed, reserve, [](RawCode code, auto callback) {
        const auto k1 = code.to_horizontal_mirror();
        if (k1 != code) {
            callback(k1);
        }
    });
}

__attribute__((noinline)) static RangesUnion extend_type_ver(RawCode seed, size_t reserve) {
    return extend_next_(seed, reserve, [](RawCode code, auto callback) {
        const auto k1 = code.to_vertical_mirror();
        // if (k1 != code) {
            callback(k1);
        // }
    });
}

__attribute__((noinline)) static RangesUnion extend_type_diag(RawCode seed, size_t reserve) {
    return extend_next_(seed, reserve, [](RawCode code, auto callback) {
        const auto k1 = code.to_vertical_mirror().to_horizontal_mirror();
        if (k1 != code) {
            callback(k1);
        }
    });
}

__attribute__((noinline)) static RangesUnion extend_type_x(RawCode seed, size_t reserve) {
    return extend_next_(seed, reserve, [](RawCode code, auto callback) {
        const auto k1 = code.to_vertical_mirror();
        callback(k1);
        const auto k2 = code.to_horizontal_mirror();
        if (k2 != code) {
            callback(k2);
            const auto p3 = k1.to_horizontal_mirror();
            callback(p3);
        }
    });
}

RangesUnion Group::cases() const {

    // TODO: add white list for single-group unions
    //       return GroupUnion::cases directly

    auto seed = CommonCode::unsafe_create(PATTERN_DATA[flat_id()] >> 23).to_raw_code();

    // NOTE: convert as RawCode directly
    if (toward_ == Toward::B) {
        seed = seed.to_horizontal_mirror();
    } else if (toward_ == Toward::C) {
        seed = seed.to_vertical_mirror();
    } else if (toward_ == Toward::D) {
        // NOTE: avoid multi convert
        seed = seed.to_vertical_mirror().to_horizontal_mirror();
    }

    RangesUnion data;
    if (mirror_type() == MirrorType::Full) {
        data = extend_type_x(seed, size());

    } else if (mirror_type() == MirrorType::Horizontal) {
        data = extend_type_hor(seed, size());

        // auto lambda = [](RawCode code, phmap::flat_hash_map<RawCode, uint64_t> &cases, std::vector<RawCode> &mirrors) {
        //     const auto k1 = code.to_horizontal_mirror();
        //     if (k1 != code) {
        //         cases.emplace(k1, 0); // TODO: contain check
        //         mirrors.emplace_back(k1);
        //     }
        // };
        //
        // data = extend_next<lambda>(seed, size());

        // data = extend_next(seed, size(), [](RawCode code, phmap::flat_hash_map<RawCode, uint64_t> &cases, std::vector<RawCode> &mirrors) {
        //     const auto k1 = code.to_horizontal_mirror();
        //     if (k1 != code) {
        //         cases.emplace(k1, 0); // TODO: contain check
        //         mirrors.emplace_back(k1);
        //     }
        // });

        // data = extend_next(seed, size(), [](RawCode code, auto callback) {
        //     const auto k1 = code.to_horizontal_mirror();
        //     if (k1 != code) {
        //         callback(k1);
        //     }
        // });

    } else if (mirror_type() == MirrorType::Vertical) {
        data = extend_type_ver(seed, size());

        // data = extend_next_(seed, size(), [](RawCode code, auto callback) {
        //     const auto k1 = code.to_vertical_mirror();
        //     if (k1 != code) { // TODO: without check
        //         callback(k1);
        //     }
        // });

    } else if (mirror_type() == MirrorType::Centro) {
        data = extend_type_diag(seed, size());

        // data = extend_next_(seed, size(), [](RawCode code, auto callback) {
        //     const auto k1 = code.to_vertical_mirror().to_horizontal_mirror();
        //     if (k1 != code) {
        //         callback(k1);
        //     }
        // });

    } else {
        data = extend_type_common(seed, size());

    }

    // auto raw_data = Group_extend_for_cases(seed, size());
    // RangesUnion data {};
    // for (auto raw_code : raw_data) {
    //     auto common_code = raw_code.to_common_code().unwrap();
    //     data.ranges(common_code >> 32).emplace_back(static_cast<uint32_t>(common_code));
    // }

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
