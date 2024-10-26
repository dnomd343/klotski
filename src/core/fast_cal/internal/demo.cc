#include "fast_cal/fast_cal.h"

#include <iostream>

#include <queue>

//#include <absl/container/flat_hash_map.h>

//#include <absl/hash/hash.h>
//#define PHMAP_USE_ABSL_HASH

//#include <robin_hood.h>
//#include <ankerl/unordered_dense.h>
//#include <tsl/robin_map.h>
#include <parallel_hashmap/phmap.h>

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

template <typename T>
class MyQueue {
public:
    explicit MyQueue(size_t reserve) {
//        vec_.reserve(reserve);
        vec_.resize(reserve);
    }

    void emplace_back(T item) {
//        vec_.emplace_back(item);
        vec_[iter_] = item;
        ++iter_;
    }

    T front() {
        return vec_[offset_];
    }

    void pop() {
        ++offset_;
    }

    bool empty() {
        return offset_ == iter_;
    }

private:
    size_t iter_ {0};
    size_t offset_ {0};
    std::vector<T> vec_ {};
};

class FCDemo {
public:
    explicit FCDemo(RawCode raw_code) : codes_(GroupUnion::from_raw_code(raw_code).max_group_size()) {
        auto reserve = GroupUnion::from_raw_code(raw_code).max_group_size();
//        codes_.reserve(reserve);
        cases_.reserve(reserve);
        codes_.emplace_back(raw_code.unwrap());
        cases_.emplace(raw_code, data_t {0, 0}); // without mask
    }

    RawCode DoCal() {
        uint64_t result = 0;
        auto core = MaskMover([this, &result](uint64_t code, uint64_t mask) {
            if (const auto match = cases_.find(code); match != cases_.end()) {
                match->second.mask |= mask; // update mask
//                match.value().mask |= mask; // update mask
                return;
            }
            cases_.emplace(code, data_t {
                .mask = mask,
                .back = codes_.front(),
//                .back = codes_[offset_].unwrap(),
            });
            codes_.emplace_back(code);

            if (((code >> 39) & 0b111) == 0b100) {
                result = code;
            }
        });

        while (!codes_.empty()) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);
            codes_.pop();

            if (result != 0) {
                return RawCode::unsafe_create(result);
            }

        }

        return RawCode::unsafe_create(0);

//        while (offset_ != codes_.size()) {
//            auto curr = codes_[offset_].unwrap();
//            core.next_cases(curr, cases_.find(curr)->second.mask);
//            ++offset_;
//
//            if (result != 0) {
//                break;
//            }
//        }

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

//	    return codes_[offset_];
    }

private:
//    uint64_t offset_ {0};
    MyQueue<uint64_t> codes_;
//    absl::flat_hash_map<uint64_t, data_t> cases_; // <code, mask>
//    robin_hood::unordered_map<uint64_t, data_t> cases_;
//    ankerl::unordered_dense::map<uint64_t, data_t> cases_;
//    tsl::robin_map<uint64_t, data_t> cases_;
    phmap::flat_hash_map<uint64_t, data_t> cases_;
};

RawCode FastCal_demo(RawCode raw_code) {
    FCDemo fc {raw_code};
    return fc.DoCal();
}
