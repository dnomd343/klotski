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

using klotski::fast_cal::FCDemo;

std::optional<RawCode> FCDemo::DoCal() {
    uint64_t result = 0;
    auto core = MaskMover([this, &result](uint64_t code, uint64_t mask) {
        if (const auto match = cases_.find(code); match != cases_.end()) {
            match->second.mask |= mask; // update mask
            return;
        }
        cases_.emplace(code, data_t {
            .mask = mask,
            .back = codes_.current(),
        });
        codes_.emplace(code);

        if (((code >> 39) & 0b111) == 0b100) {
            result = code;
        }
    });

    while (!codes_.is_ending()) {
        auto curr = codes_.current();
        core.next_cases(curr, cases_.find(curr)->second.mask);

        codes_.next();
        if (result != 0) {
            return RawCode::unsafe_create(result);
        }
    }
    return std::nullopt;
}

std::vector<RawCode> FCDemo::DoCalMulti() {
    bool stop_flag = false;
    std::vector<RawCode> results {};

    auto core = MaskMover([this, &stop_flag, &results](uint64_t code, uint64_t mask) {
        if (const auto match = cases_.find(code); match != cases_.end()) {
            match->second.mask |= mask; // update mask
            return;
        }
        cases_.emplace(code, data_t {
            .mask = mask,
            .back = codes_.current(),
        });
        codes_.emplace(code);

        if (((code >> 39) & 0b111) == 0b100) {
            stop_flag = true;
            results.emplace_back(RawCode::unsafe_create(code));
        }
    });

    while (!codes_.is_ending()) {
        auto curr = codes_.current();
        core.next_cases(curr, cases_.find(curr)->second.mask);

        codes_.next();
        if (codes_.is_new_layer() && stop_flag) {
            // TODO: fix when solutions at last layer
            return results;
        }

    }
    return {};
}

std::vector<RawCode> FCDemo::DoCalFurthest() {
    auto core = MaskMover([this](uint64_t code, uint64_t mask) {
        if (const auto match = cases_.find(code); match != cases_.end()) {
            match->second.mask |= mask; // update mask
            return;
        }
        cases_.emplace(code, data_t {
            .mask = mask,
            .back = codes_.current(),
        });
        codes_.emplace(code);
    });

    while (true) {
        auto curr = codes_.current();
        core.next_cases(curr, cases_.find(curr)->second.mask);

        codes_.next();
        if (codes_.is_ending()) {
            return codes_.layer_cases() | std::views::transform([](uint64_t code) {
                return RawCode::unsafe_create(code);
            }) | std::ranges::to<std::vector>();
        }
    }
}

RawCode FastCal_demo(RawCode raw_code) {
    klotski::fast_cal::FCDemo fc {raw_code};
//    return fc.DoCal().value();

//    auto tmp = fc.DoCal();
//    std::cout << tmp.value().to_common_code() << std::endl;

//    auto tmp = fc.DoCalMulti();
//    for (const auto x : tmp) {
//        std::cout << x.to_common_code() << std::endl;
//    }

    auto tmp = fc.DoCalFurthest();
//    for (const auto x : tmp) {
//        std::cout << x.to_common_code() << std::endl;
//    }

    return RawCode::unsafe_create(0);
}
