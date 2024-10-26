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
        data_.resize(reserve);
    }

    void emplace_back(T item) {
        data_[queue_end_] = item;
        ++queue_end_;
    }

    T front() {
        return data_[queue_begin_];
    }

    void pop_and_try_layer_switch() {
        ++queue_begin_;
        try_layer_switch();
    }

    [[nodiscard]] bool is_ending() const {
        return queue_begin_ == queue_end_;
    }

    void try_layer_switch() {
        if (queue_begin_ == layer_end_ && !is_ending()) {
            layer_begin_ = layer_end_;
            layer_end_ = queue_end_;
        }
    }

    [[nodiscard]] bool is_layer_switched() const {
        return queue_begin_ == layer_begin_;
    }

    std::vector<T> get_curr_layer() {
        std::vector<T> layer_cases;
        for (size_t offset = layer_begin_; offset < layer_end_; ++offset) {
            layer_cases.emplace_back(data_[offset]);
        }
        return layer_cases;
    }

private:
    size_t queue_begin_ {0};
    size_t queue_end_ {0};

    size_t layer_begin_ {0};
    size_t layer_end_ {1};

    std::vector<T> data_ {};
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

    std::optional<RawCode> DoCal() {
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

        while (!codes_.is_ending()) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);

            codes_.pop_and_try_layer_switch();
            if (result != 0) {
                return RawCode::unsafe_create(result);
            }
        }
        return std::nullopt;
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

        while (!codes_.is_ending()) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);

            codes_.pop_and_try_layer_switch();
            if (codes_.is_layer_switched() && stop_flag) {
                return results;
            }

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

        while (true) {
            auto curr = codes_.front();
            core.next_cases(curr, cases_.find(curr)->second.mask);

            codes_.pop_and_try_layer_switch();
            if (codes_.is_ending()) {
                return codes_.get_curr_layer() | std::views::transform([](uint64_t code) {
                    return RawCode::unsafe_create(code);
                }) | std::ranges::to<std::vector>();
            }
        }
    }

private:
    MyQueue<uint64_t> codes_;
    phmap::flat_hash_map<uint64_t, data_t> cases_;
};

RawCode FastCal_demo(RawCode raw_code) {
    FCDemo fc {raw_code};
    return fc.DoCal().value();

//    auto tmp = fc.DoCal();
//    std::cout << tmp.to_common_code() << std::endl;

//    auto tmp = fc.DoCalMulti();
//    for (const auto x : tmp) {
//        std::cout << x.to_common_code() << std::endl;
//    }

//    auto tmp = fc.DoCalFurthest();
//    for (const auto x : tmp) {
//        std::cout << x.to_common_code() << std::endl;
//    }

    return RawCode::unsafe_create(0);
}
