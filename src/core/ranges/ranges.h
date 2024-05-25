#pragma once

#include <vector>
#include <cstdint>

#include "utils/utility.h"

namespace klotski::cases {

// void spawn_ranges(std::vector<uint32_t> &ranges, int n, int n_2x1, int n_1x1);

// std::vector<uint32_t> basic_ranges();

// TODO: should we inherit on `std::vector<uint32_t>` ?
class Ranges {
public:
    void spawn_more(int n, int n_2x1, int n_1x1);

    using iterator = std::vector<uint32_t>::iterator;
    using size_type = std::vector<uint32_t>::size_type;
    using const_iterator = std::vector<uint32_t>::const_iterator;

    using value_type = std::vector<uint32_t>::value_type;

    using reference = std::vector<uint32_t>::reference;

    using const_reference = std::vector<uint32_t>::const_reference;

    iterator begin() {
        return ranges_.begin();
    }

    iterator end() {
        return ranges_.end();
    }

    [[nodiscard]] const_iterator begin() const {
        return ranges_.begin();
    }

    [[nodiscard]] const_iterator end() const {
        return ranges_.end();
    }

    void clear() {
        ranges_.clear();
    }

    [[nodiscard]] size_type size() const {
        return ranges_.size();
    }

    void reserve(const size_type cap) {
        ranges_.reserve(cap);
    }

    void with_head(int head, Ranges &release) const;

    void reverse() {
        for (auto &x : ranges_) {
            x = range_reverse(x); // flip every 2-bit
        }
    }

    const_reference operator[](const size_type n) const {
        return ranges_[n];
    }

    [[nodiscard]] const value_type* data() const {
        return ranges_.data();
    }

// private:
    std::vector<uint32_t> ranges_ {};
};

} // namespace klotski::cases
