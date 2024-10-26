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

struct data_t {
    uint64_t mask;
    uint64_t back;
};

template <typename T>
class MyQueue {
public:
    explicit MyQueue(size_t reserve) {
        vec_.resize(reserve);
    }

    void emplace_back(T item) {
        vec_[back_] = item;
        ++back_;
    }

    T front() {
        return vec_[front_];
    }

    void pop() {
        ++front_;
    }

    bool empty() {
        return front_ == back_;
    }

    bool is_ending() {
        return front_ == back_;
    }

    void layer_logic() {
        if (front_ == layer_end_) {

            // std::cout << std::format("[{}, {}) <- {}\n", layer_begin, layer_end, layer_end - layer_begin);

            if (layer_end_ == back_) {
                // maybe reach ending -> not switch
//                std::cout << "reach ending" << std::endl;
            } else {
                layer_begin_ = layer_end_;
                layer_end_ = back_;
            }

        }
    }

//private:

    size_t layer_begin_ {0};
    size_t layer_end_ {1};

    size_t back_ {0};
    size_t front_ {0};
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
            codes_.layer_logic();

            if (result != 0) {
                return RawCode::unsafe_create(result);
            }
        }
        return RawCode::unsafe_create(0);
    }

    std::vector<RawCode> DoCalMulti() {
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
                results.emplace_back(RawCode::unsafe_create(code));
            }
        });

        while (!codes_.empty()) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);
            codes_.pop();

            if (codes_.front_ == codes_.layer_end_) {
                if (stop_flag) {
                    return results;
                }
            }

            codes_.layer_logic();

        }
        return {};
    }

    std::vector<RawCode> DoCalFurthest() {

        auto core = MaskMover([this](uint64_t code, uint64_t mask) {
            if (const auto match = cases_.find(code); match != cases_.end()) {
                match->second.mask |= mask; // update mask
                return;
            }
            cases_.emplace(code, data_t {
                .mask = mask,
                .back = codes_.front(),
            });
            codes_.emplace_back(code);
        });

        while (!codes_.is_ending()) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);
            codes_.pop();

            codes_.layer_logic();

            if (codes_.front_ == codes_.layer_end_) {
                if (codes_.layer_end_ == codes_.back_) {
                    std::vector<RawCode> codes;
                    for (size_t offset = codes_.layer_begin_; offset < codes_.layer_end_; ++offset) {
                        codes.emplace_back(RawCode::unsafe_create(codes_.vec_[offset]));
                    }
                    return codes;
                }
            }

        }
        return {};
    }

private:
    MyQueue<uint64_t> codes_;
    phmap::flat_hash_map<uint64_t, data_t> cases_;
};

RawCode FastCal_demo(RawCode raw_code) {
    FCDemo fc {raw_code};
    return fc.DoCal();

//    auto tmp = fc.DoCal();
//    std::cout << tmp << std::endl;

//    auto tmp = fc.DoCalMulti();
//    for (auto code : tmp) {
//        std::cout << code << std::endl;
//    }

//    auto tmp = fc.DoCalFurthest();
//    for (auto x : tmp) {
//        std::cout << x << std::endl;
//    }

//    return RawCode::unsafe_create(0);
}
