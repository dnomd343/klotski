#include "ranges/ranges.h"
#include "common_code/common_code.h"

using klotski::cases::Ranges;
using klotski::codec::CommonCode;
using klotski::cases::RangesUnion;

static constexpr auto heads = std::to_array<uint64_t>({
    0x0, 0x1, 0x2, 0x4, 0x5, 0x6,
    0x8, 0x9, 0xA, 0xC, 0xD, 0xE,
});

void Ranges::reverse() {
    for (auto &x : *this) {
        x = range_reverse(x);
    }
}

Ranges& Ranges::operator+=(const Ranges &ranges) {
    this->insert(this->end(), ranges.begin(), ranges.end());
    return *this;
}

RangesUnion& RangesUnion::operator+=(const RangesUnion &ranges_union) {
    for (const auto head : heads) {
        // (*this)[head] += ranges_union[head];
        std::array<Ranges, 16>::operator[](head) += ranges_union.std::array<Ranges, 16>::operator[](head);
    }
    return *this;
}

std::vector<CommonCode> RangesUnion::codes() const {
    std::vector<CommonCode> codes;
    codes.reserve(size());
    for (const auto head : heads) {
        // for (const auto range : (*this)[head]) {
        for (const auto range : ranges(head)) {
            codes.emplace_back(CommonCode::unsafe_create(head << 32 | range));
        }
    }
    // TODO: try using std::views
    return codes;
}

// TODO: move to `.inl` file
size_t RangesUnion::size() const {
    size_type size = 0;
    for (const auto head : heads) {
        size += std::array<Ranges, 16>::operator[](head).size();
        // size += (*this)[head].size();
    }
    return size;
}

uint32_t RangesUnion::operator[](size_type index) const {
    size_t head = 0;
    for (;;) {
        if (index >= std::array<Ranges, 16>::operator[](head).size()) {
            index -= std::array<Ranges, 16>::operator[](head).size();
            ++head;
        } else {
            break;
        }
    }
    return std::array<Ranges, 16>::operator[](head)[index];
}
