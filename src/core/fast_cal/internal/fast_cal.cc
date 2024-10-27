#include "utils/common.h"
#include "fast_cal/fast_cal.h"

using klotski::BLOCK_MSK;
using klotski::BLOCK_CE_2x2;

using klotski::codec::RawCode;
using klotski::mover::MaskMover;
using klotski::fast_cal::FastCal;

// ----------------------------------------------------------------------------------------- //

void FastCal::build_all() {
    auto mover = MaskMover([this](const RawCode code, const uint64_t mask) {
        try_emplace(code, mask);
    });
    while (!seeker_.is_ending()) {
        spawn_next(mover);
    }
}

std::vector<RawCode> FastCal::furthest() {
    if (!seeker_.is_ending()) {
        build_all();
    }
    return seeker_.last_layer();
}

// ----------------------------------------------------------------------------------------- //

std::vector<std::vector<RawCode>> FastCal::exports() const {
    return seeker_.all_layers();
}

std::vector<RawCode> FastCal::backtrack(RawCode code) const {
    if (const auto match = cases_.find(code); match == cases_.end()) {
        return {}; // case not found
    }
    std::vector<RawCode> path;
    while (code != 0) {
        path.emplace_back(code);
        code = cases_.find(code)->second.parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// ----------------------------------------------------------------------------------------- //

static KLSK_INLINE bool is_solved(const RawCode raw_code) {
    return ((raw_code.unwrap() >> 39) & BLOCK_MSK) == BLOCK_CE_2x2;
}

std::optional<RawCode> FastCal::solve() {
    if (is_solved(seeker_.current())) {
        return seeker_.current(); // root case solved
    }

    RawCode solution {nil};
    auto mover = MaskMover([this, &solution](const RawCode code, const uint64_t mask) {
        if (try_emplace(code, mask) && is_solved(code)) {
            solution = code;
        }
    });

    while (!seeker_.is_ending()) {
        spawn_next(mover);
        if (solution != nil) {
            return solution;
        }
    }
    return std::nullopt;
}

std::optional<RawCode> FastCal::search(std::function<bool(RawCode)> &&match) {
    if (match(seeker_.current())) {
        return seeker_.current(); // root case matched
    }

    RawCode target {nil};
    auto mover = MaskMover([this, &target, match = std::move(match)](const RawCode code, const uint64_t mask) {
        if (try_emplace(code, mask) && match(code)) {
            target = code;
        }
    });

    while (!seeker_.is_ending()) {
        spawn_next(mover);
        if (target != nil) {
            return target;
        }
    }
    return std::nullopt;
}

std::vector<RawCode> FastCal::solve_multi() {
    if (is_solved(seeker_.current())) {
        return {seeker_.current()}; // root case solved
    }

    bool stop_flag = false;
    std::vector<RawCode> results {};
    auto mover = MaskMover([this, &stop_flag, &results](const RawCode code, const uint64_t mask) {
        if (try_emplace(code, mask) && is_solved(code)) {
            stop_flag = true;
            results.emplace_back(code);
        }
    });

    while (!seeker_.is_ending()) {
        spawn_next(mover);
        if (stop_flag && (seeker_.is_new_layer() || seeker_.is_ending())) {
            return results;
        }
    }
    return {};
}

// ----------------------------------------------------------------------------------------- //
