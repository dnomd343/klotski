#include "fast_cal/fast_cal.h"

#include <iostream>

//#include <absl/container/flat_hash_map.h>

#include <parallel_hashmap/phmap.h>

#include "mover/mover.h"
#include "group/group.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;
using klotski::cases::GroupUnion;

using klotski::fast_cal::FastCalPro;

static KLSK_INLINE bool is_solved(uint64_t raw_code) {
    return ((raw_code >> 39) & 0b111) == 0b100;
}

std::optional<RawCode> FastCalPro::solve() {
    // TODO: check root case

    uint64_t solution = 0;
    auto mover = MaskMover([this, &solution](uint64_t code, uint64_t mask) {
        if (try_emplace(code, mask) && is_solved(code)) {
            solution = code;
        }
    });
    while (!codes_.is_ending()) {
        spawn_next(mover);
        if (solution != 0) {
            return RawCode::unsafe_create(solution);
        }
    }
    return std::nullopt;
}

std::optional<RawCode> FastCalPro::achieve(std::function<bool(RawCode)> &&match) {
    // TODO: check root case

    uint64_t target = 0;
    auto mover = MaskMover([this, &target, match = std::move(match)](uint64_t code, uint64_t mask) {
        if (try_emplace(code, mask) && match(RawCode::unsafe_create(code))) {
            target = code;
        }
    });
    while (!codes_.is_ending()) {
        spawn_next(mover);
        if (target != 0) {
            return RawCode::unsafe_create(target);
        }
    }
    return std::nullopt;
}

std::vector<RawCode> FastCalPro::solve_multi() {
    // TODO: check root case

    bool stop_flag = false;
    std::vector<RawCode> results {};

    auto mover = MaskMover([this, &stop_flag, &results](uint64_t code, uint64_t mask) {
        if (try_emplace(code, mask) && is_solved(code)) {
            stop_flag = true;
            results.emplace_back(RawCode::unsafe_create(code));
        }
    });

    while (!codes_.is_ending()) {
        spawn_next(mover);
        if (codes_.is_new_layer() && stop_flag) {
            // TODO: fix when solutions at last layer
            return results;
        }

    }
    return {};
}

std::vector<RawCode> FastCalPro::furthest() {
    auto mover = MaskMover([this](uint64_t code, uint64_t mask) {
        try_emplace(code, mask);
    });
    while (true) {
        spawn_next(mover);
        if (codes_.is_ending()) {
            return codes_.layer_cases() | std::views::transform([](uint64_t code) {
                return RawCode::unsafe_create(code);
            }) | std::ranges::to<std::vector>();
        }
    }
}

RawCode FastCal_demo(RawCode raw_code) {
    klotski::fast_cal::FastCalPro fc {raw_code};
    return fc.solve().value();

//    auto tmp = fc.solve();
//    std::cout << tmp.value().to_common_code() << std::endl;

//    auto tmp = fc.solve_multi();
//    for (const auto x : tmp) {
//        std::cout << x.to_common_code() << std::endl;
//    }

//    auto tmp = fc.furthest();
//    for (const auto x : tmp) {
//        std::cout << x.to_common_code() << std::endl;
//    }

//    auto tmp = fc.achieve([](RawCode r) {
//        return r == 0x7F87E0E5BFFF492;
//    });
//    std::cout << tmp.value().to_common_code() << std::endl;

//    return RawCode::unsafe_create(0);
}
