#include <iostream>
#include <format>

#include "common_code/common_code.h"

using klotski::codec::CommonCode;

bool CommonCode::is_mirror(uint64_t common_code) {

    int head = common_code >> 32;
    uint32_t ranges = range_reverse(common_code);

    // std::cout << "head = " << head << std::endl;
    // std::cout << "ranges = " << std::format("{:08X}", ranges) << std::endl;

    if (head % 4 != 1) {
        return false;
    }

    std::array<bool, 6> state {}; // mark emtry/half-full

    if (head == 1) {
        state.at(0) = true;
        state.at(1) = true;
    } else if (head == 5) {
        state.at(1) = true;
        state.at(2) = true;
    } else if (head == 9) {
        state.at(2) = true;
        state.at(3) = true;
    } else if (head == 13) {
        state.at(3) = true;
        state.at(4) = true;
    }

    int working_line = 0;

    while (1 == 1) {

        if (working_line > 4) {
            // std::cout << "reach end line" << std::endl;
            break;
        }

        // std::cout << std::endl;
        // std::cout << "working_line: " << working_line << std::endl;
        // std::cout << "state: " << std::format("{}", state) << std::endl;

        if (!state.at(working_line)) { // empty line
            // std::cout << "empty working line" << std::endl;

            /// simple single line
            bool is_single_line = false;
            if ((ranges & 0b1111) == 0b0101) {
                ranges >>= 4;
                is_single_line = true;
            } else {
                auto tmp = ranges & 0b111111;
                if (tmp == 0b110111 || tmp == 0b000100) {
                    ranges >>= 6;
                    is_single_line = true;
                } else {
                    auto tmp_ = ranges & 0b11111111;
                    if (tmp_ == 0b00000000 || tmp_ == 0b11111111 || tmp_ == 0b00111100 || tmp_ == 0b11000011) {
                        ranges >>= 8;
                        is_single_line = true;
                    }
                }
            }
            if (is_single_line) {
                // std::cout << "simple single line" << std::endl;
                ++working_line; // next line
                continue;
            }

            /// half-double line
            bool is_half_double_line = false;
            if ((ranges & 0b111111) == 0b100110) {
                ranges >>= 6;
                is_half_double_line = true;
            } else {
                auto tmp = ranges & 0b11111111;
                if (tmp == 0b10111110 || tmp == 0b11101011 || tmp == 0b00101000 || tmp == 0b10000010) {
                    ranges >>= 8;
                    is_half_double_line = true;
                }
            }
            if (is_half_double_line) {
                // std::cout << "half double line" << std::endl;
                if (state.at(working_line + 1)) {
                    working_line += 2; // next 2 lines
                } else {
                    state.at(working_line + 1) = true;
                    ++working_line; // next line
                }
                continue;
            }

            /// full-double line
            if ((ranges & 0b11111111) == 0b10101010) {
                // std::cout << "full double line" << std::endl;
                ranges >>= 8;
                working_line += 2;
                continue;
            }

            // std::cout << "not mirror layout" << std::endl;
            return false;

        } else { // half-full line

            // std::cout << "half-full working line" << std::endl;

            /// simple single line
            bool is_simple_line = false;
            if ((ranges & 0b11) == 0b01) {
                ranges >>= 2;
                is_simple_line = true;
            } else {
                auto tmp = ranges & 0b1111;

                if (tmp == 0b0000 || tmp == 0b1111) {
                    ranges >>= 4;
                    is_simple_line = true;
                }
            }
            if (is_simple_line) {
                // std::cout << "simple single line" << std::endl;
                ++working_line;
                continue;
            }

            /// half-double line
            if ((ranges & 0b1111) == 0b1010) {
                // std::cout << "half double line" << std::endl;
                ranges >>= 4;
                if (state.at(working_line + 1)) {
                    working_line += 2; // next 2 lines
                } else {
                    state.at(working_line + 1) = true;
                    ++working_line; // next line
                }
                continue;
            }

            // std::cout << "not mirror layout" << std::endl;
            return false;

        }

    }

    return true;
}
