#include "mover/mover.h"
#include "group/group.h"
#include "fast_cal/fast_cal.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;
using klotski::cases::GroupUnion;

using klotski::fast_cal::FastCalPro;

static KLSK_INLINE bool is_solved(RawCode raw_code) {
    return ((raw_code.unwrap() >> 39) & 0b111) == 0b100;
}

std::optional<RawCode> FastCalPro::solve() {
    // if (is_solved(root_.unwrap())) {
    //     return root_;
    // }

    uint64_t solution = 0;
    auto mover = MaskMover([this, &solution](RawCode code, uint64_t mask) {
        if (try_emplace(code, mask) && is_solved(code)) {
            solution = code.unwrap();
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

std::optional<RawCode> FastCalPro::search(std::function<bool(RawCode)> &&match) {
    // TODO: check root case

    uint64_t target = 0;
    auto mover = MaskMover([this, &target, match = std::move(match)](RawCode code, uint64_t mask) {
        if (try_emplace(code, mask) && match(code)) {
            target = code.unwrap();
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

    auto mover = MaskMover([this, &stop_flag, &results](RawCode code, uint64_t mask) {
        if (try_emplace(code, mask) && is_solved(code)) {
            stop_flag = true;
            results.emplace_back(code);
        }
    });

    while (!codes_.is_ending()) {
        spawn_next(mover);
        if (stop_flag && (codes_.is_new_layer() || codes_.is_ending())) {
            return results;
        }

    }
    return {};
}

void FastCalPro::build() {
    auto mover = MaskMover([this](RawCode code, uint64_t mask) {
        try_emplace(code, mask);
    });
    while (!codes_.is_ending()) {
        spawn_next(mover);
    }
}

std::vector<RawCode> FastCalPro::furthest() {
    if (!codes_.is_ending()) {
        build();
    }
    return codes_.last_layer();
}

std::vector<RawCode> FastCalPro::backtrack(RawCode code) const {
    if (const auto match = cases_.find(code); match == cases_.end()) {
        return {}; // case not found
    }
    std::vector<RawCode> path;
    while (code != 0) {
        path.emplace_back(code);
        code = cases_.find(code)->second.back;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<std::vector<RawCode>> FastCalPro::exports() const {
    return codes_.all_layers();
}
