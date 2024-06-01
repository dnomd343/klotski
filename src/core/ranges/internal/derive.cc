#include <iostream>

#include <format>

#include "utils/utility.h"
#include "ranges/ranges.h"

using klotski::cases::Ranges;

int Ranges::check(const int head, uint32_t range) {
    KLSK_ASSUME(head >= 0 && head < 16 && head % 4 != 3);
    uint32_t flags = 0b110011 << head; // fill 2x2 block
    for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
        const auto num = std::countr_one(flags);
        addr += num; // next unfilled block
        flags >>= num;
        switch (range & 0b11) {
            case 0b00: // space
            case 0b11: // 1x1 block
                flags |= 0b1;
                continue;
            case 0b01: // 1x2 block
                if (flags & 0b10 || addr % 4 == 3) { // invalid case
                    return offset; // broken offset
                }
                flags |= 0b11;
                continue;
            case 0b10: // 2x1 block
                if (flags & 0b10000 || addr > 15) { // invalid case
                    return offset; // broken offset
                }
                flags |= 0b10001;
        }
    }
    return 0; // pass check
}

// int check_range(const int head, uint32_t range) {
//     KLSK_ASSUME(head >= 0 && head < 16 && head % 4 != 3);
//     uint32_t flags = 0b110011 << head; // fill 2x2 block
//     for (int addr = 0, offset = 1; range; range >>= 2, ++offset) { // traverse every 2-bit
//         const auto num = std::countr_one(flags);
//         addr += num; // next unfilled block
//         flags >>= num;
//         switch (range & 0b11) {
//             case 0b00: // space
//             case 0b11: // 1x1 block
//                 flags |= 0b1;
//                 continue;
//             case 0b01: // 1x2 block
//                 if (flags & 0b10 || addr % 4 == 3) { // invalid case
//                     return offset; // broken offset
//                 }
//                 flags |= 0b11;
//                 continue;
//             case 0b10: // 2x1 block
//                 if (flags & 0b10000 || addr > 15) { // invalid case
//                     return offset; // broken offset
//                 }
//                 flags |= 0b10001;
//         }
//     }
//     return 0; // pass check
// }

void Ranges::derive(const int head, Ranges &output) const {

    // Ranges reversed {*this};
    //
    // for (auto &x : reversed) {
    //     x = klotski::range_reverse(x);
    // }

    // std::cout << "last = " << range_reverse(this->back()) << std::endl;

    // auto &ranges = *this;

    // uint32_t min_next = 0;
    uint32_t last_val = range_reverse(this->back());

    for (uint32_t index = 0; index < size(); ++index) {
        if (const auto offset = check(head, (*this)[index])) { // invalid case
            uint32_t tmp = 1U << (32 - offset * 2); // distance to next possible range
            ///         !! <- broken
            /// ( xx xx xx ) xx xx xx ... [reversed range]
            ///         +1   00 00 00 ...     (delta)
            tmp += range_reverse((*this)[index]) & ~(tmp - 1);

            // std::cout << "min next = " << tmp << std::endl;

            // if (min_next > tmp) {
            //     std::cout << "error" << std::endl;
            // }
            auto min_next = tmp;

            // const int tmp = (16 - offset) * 2;
            // const uint32_t min_next = ((range_reverse((*this)[index]) >> tmp) + 1) << tmp;

            if (min_next > last_val) {
                // std::cout << "get it" << std::endl;
                break;
                // return;
            }

            // TODO: overflow here in some type_id
            // TODO:   -> tmp > range[-1]
            // TODO: maybe using binary search here

            while (range_reverse((*this)[++index]) < min_next) {} // located next range
            --index;
            continue;
        }
        output.emplace_back(range_reverse((*this)[index])); // release valid case
    }

    // std::cout << "min_next = " << min_next << " | last_val = " << last_val << std::endl;

    // std::cout << (min_next <= last_val) << std::endl;

}
