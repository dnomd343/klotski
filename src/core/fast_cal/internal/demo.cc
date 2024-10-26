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
//    MyQueue() = default;

    explicit MyQueue(size_t reserve) {
        vec_.resize(reserve);
    }

//    void reserve(size_t size) {
//        vec_.resize(size);
//    }

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

//private:
    size_t iter_ {0};
    size_t offset_ {0};
    std::vector<T> vec_ {};
};

class FCDemo {
public:
    explicit FCDemo(RawCode raw_code) : codes_(GroupUnion::from_raw_code(raw_code).max_group_size()) {
        // TODO: build codes_ with reserve size
        auto reserve = GroupUnion::from_raw_code(raw_code).max_group_size();
//        codes_.reserve(reserve);
//        cases_.reserve(static_cast<size_t>(reserve * 1.56));
        cases_.reserve(static_cast<size_t>(25955 * 1.56));
        codes_.emplace_back(raw_code.unwrap());
        cases_.emplace(raw_code, data_t {0, 0}); // without mask
    }

    RawCode DoCal() {
        uint64_t result = 0;
        auto core = MaskMover([this, &result](uint64_t code, uint64_t mask) {
            if (const auto match = cases_.find(code); match != cases_.end()) {
                match->second.mask |= mask; // update mask
                return;
            }
            cases_.emplace(code, data_t {
                .mask = mask,
                .back = codes_.front(),
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
    }

    std::vector<RawCode> DoCalMulti() {
//        uint64_t result = 0;
        bool stop_flag = false;
        std::vector<RawCode> results {};

        auto core = MaskMover([this, &stop_flag, &results](uint64_t code, uint64_t mask) {
            if (const auto match = cases_.find(code); match != cases_.end()) {
                match->second.mask |= mask; // update mask
                return;
            }
            cases_.emplace(code, data_t {
                .mask = mask,
                .back = codes_.front(),
            });
            codes_.emplace_back(code);

            if (((code >> 39) & 0b111) == 0b100) {
                stop_flag = true;
//                result = code;
                results.emplace_back(RawCode::unsafe_create(code));
            }
        });

        size_t layer_end = 1;

        while (!codes_.empty()) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);
            codes_.pop();

            if (codes_.offset_ == layer_end) {
//                std::cout << "layer: " << codes_.offset_ << std::endl;
                layer_end = codes_.iter_;
                if (stop_flag) {
                    return results;
                }
            }

//            if (result != 0) {
//                return RawCode::unsafe_create(result);
//            }
        }
        return {};
    }

private:
    MyQueue<uint64_t> codes_;
    phmap::flat_hash_map<uint64_t, data_t> cases_;
};

RawCode FastCal_demo(RawCode raw_code) {
    FCDemo fc {raw_code};
//    return fc.DoCal();
    auto tmp = fc.DoCalMulti();
//    for (auto code : tmp) {
//        std::cout << code << std::endl;
//    }
    return tmp[0];
}
