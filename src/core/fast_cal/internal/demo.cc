#include "fast_cal/fast_cal.h"

#include <iostream>

#include <queue>

#include <absl/container/flat_hash_map.h>

#include "mover/mover.h"
#include "group/group.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

using klotski::mover::MaskMover;
using klotski::cases::GroupUnion;

struct data_t {
    uint64_t mask;
    uint64_t back;
};

// TODO: try wrapper as custom `std::queue`

class FCDemo {
public:
    explicit FCDemo(RawCode raw_code) {
        auto reserve = GroupUnion::from_raw_code(raw_code).max_group_size();
        codes_.reserve(reserve);
        cases_.reserve(reserve);
        codes_.emplace_back(raw_code);
        cases_.emplace(raw_code, data_t {0, 0}); // without mask
    }

    RawCode DoCal() {
//        bool stop_flag = false;
        uint64_t result = 0;
        auto core = MaskMover([this, &result](uint64_t code, uint64_t mask) {
            if (const auto match = cases_.find(code); match != cases_.end()) {
                match->second.mask |= mask; // update mask
                return;
            }
            cases_.emplace(code, data_t {
                .mask = mask,
                .back = codes_[offset_].unwrap(),
            });
            codes_.emplace_back(RawCode::unsafe_create(code));

            if (((code >> 39) & 0b111) == 0b100) {
//                stop_flag = true;
                result = code;
            }
        });

        while (offset_ != codes_.size()) {
            auto curr = codes_[offset_].unwrap();
            core.next_cases(curr, cases_.find(curr)->second.mask);
            ++offset_;

            if (result != 0) {
                break;
            }
        }

//        std::vector<RawCode> path;
//        auto code = result;
//        while (true) {
//            if (code == 0) {
//                break;
//            }
//            path.emplace_back(RawCode::unsafe_create(code));
//            code = cases_.find(code)->second.back;
//        }
//        std::reverse(path.begin(), path.end());
//        for (auto step : path) {
//            std::cout << step << std::endl;
//        }
//        std::cout << path.size() << std::endl;

	return codes_[offset_];
    }

private:
    uint64_t offset_ {0};
    std::vector<RawCode> codes_;
    absl::flat_hash_map<uint64_t, data_t> cases_; // <code, mask>
};

RawCode FastCal_demo(RawCode raw_code) {

    FCDemo fc {raw_code};
    return fc.DoCal();

//    std::queue<RawCode> cache;
    std::vector<RawCode> codes;
    absl::flat_hash_map<uint64_t, data_t> cases; // <code, mask>
    auto reserve = GroupUnion::from_raw_code(raw_code).max_group_size();
    codes.reserve(reserve);
    cases.reserve(reserve);

    uint64_t offset = 0;
    bool stop_flag = false;
    auto core = MaskMover([&codes, &cases, &offset, &stop_flag](uint64_t code, uint64_t mask) {
        if (const auto match = cases.find(code); match != cases.end()) {
            match->second.mask |= mask; // update mask
            return;
        }
        cases.emplace(code, data_t {
            .mask = mask,
            .back = codes[offset].unwrap(),
//            .back = cache.front().unwrap(),
        });
        codes.emplace_back(RawCode::unsafe_create(code));
//        cache.emplace(RawCode::unsafe_create(code));
        if (((code >> 39) & 0b111) == 0b100) {
            stop_flag = true;
        }
    });

    codes.emplace_back(raw_code);
//    cache.emplace(raw_code);
    cases.emplace(raw_code, data_t {0, 0}); // without mask

    while (offset != codes.size()) {
        auto curr = codes[offset].unwrap();
        core.next_cases(curr, cases.find(curr)->second.mask);
        ++offset;

        if (stop_flag) {
            break;
        }

    }

//    while (!cache.empty()) {
//        if (((cache.front().unwrap() >> 39) & 0b111) == 0b100) {
//            break;
//        }
//        uint64_t curr = cache.front().unwrap();
//        core.next_cases(curr, cases.find(curr)->second.mask);
//        cache.pop();
//        if (stop_flag) {
//            break;
//        }
//    }

//    std::vector<RawCode> path;
//    auto code = cache.front().unwrap();
//    while (true) {
//        if (code == 0) {
//            break;
//        }
//        path.emplace_back(RawCode::unsafe_create(code));
//        code = cases.find(code)->second.back;
//    }
//    std::reverse(path.begin(), path.end());
//    for (auto step : path) {
//        std::cout << step << std::endl;
//    }
//    std::cout << path.size() << std::endl;

    return codes[offset];
//    return cache.front();

}
