#include "case.h"
#include "common.h"
#include <algorithm>

bool check_case(int head, uint32_t range);
void build_base_range(std::vector<uint32_t> &base_range);
void gen_range(std::vector<uint32_t> &release, int n1, int n2, int n3, int n4);

void find_all_case(std::vector<uint64_t> *all_case) {
    all_case->clear();
    std::vector<uint32_t> base_range;
    build_base_range(base_range);
    for (int head = 0; head < 16; ++head) { // address for 2x2 block
        if (head % 4 != 3) {
            uint64_t prefix = int64_t(head) << 32;
            for (auto range : base_range) { // combine 2x2 address and range
                if (check_case(head, range)) {
                    binary_reverse(range);
                    all_case->emplace_back(prefix | range);
                }
            }
        }
    }
}

void build_base_range(std::vector<uint32_t> &base_range) {
    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ (20 - 4 - 2) / 2
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 1x2 block
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block
                gen_range(base_range, (16 - n * 2 - n_1x1), (n - n_2x1), n_2x1, n_1x1);
            }
        }
    }
    for (auto &bin : base_range) {
        binary_reverse(bin);
    }
    std::sort(base_range.begin(), base_range.end());
    for (auto &bin : base_range) {
        binary_reverse(bin);
    }
}

void gen_range(std::vector<uint32_t> &release, int n1, int n2, int n3, int n4) {
    std::vector<uint32_t> cache_1, cache_2;

    int length = n1 + n2;
    for (uint32_t bin = 0; bin < (0x1 << length); ++bin) {
        if (binary_num(bin, length) == n2) { // match binary with `n2` non-zero bit
            uint32_t range = 0;
            for (int i = 0; i < length; ++i) { // generate range base on binary value
                (range <<= 2) |= ((bin >> i) & 0x1);
            }
            cache_1.emplace_back(range); // insert into cache level 1
        }
    }

    length = n1 + n2 + n3;
    for (uint32_t bin = 0; bin < (0x1 << length); ++bin) {
        if (binary_num(bin, length) == n3) { // match binary with `n3` non-zero bit
            for (auto base : cache_1) { // traverse cache level 1
                uint32_t range = 0;
                for (int i = 0; i < length; ++i) { // generate range base on binary value
                    if ((bin >> i) & 1) { // non-zero bit
                        (range <<= 2) |= 0x2;
                    } else { // zero bit
                        (range <<= 2) |= (base & 0x3);
                        base >>= 2;
                    }
                }
                cache_2.emplace_back(range); // insert into cache level 2
            }
        }
    }

    length = n1 + n2 + n3 + n4;
    for (uint32_t bin = 0; bin < (0x1 << length); ++bin) {
        if (binary_num(bin, length) == n4) { // match binary with `n4` non-zero bit
            for (auto base : cache_2) { // traverse cache level 2
                uint32_t range = 0;
                for (int i = 0; i < length; ++i) { // generate range base on binary value
                    if ((bin >> i) & 1) { // non-zero bit
                        (range <<= 2) |= 0x3;
                    } else { // zero bit
                        (range <<= 2) |= (base & 0x3);
                        base >>= 2;
                    }
                }
                release.emplace_back(range); // insert into release
            }
        }
    }
}

bool check_case(int head, uint32_t range) { // whether case is valid
    uint32_t status = 0x33 << head;
    for (int addr = 0; range; range >>= 2) {
        while (status >> addr & 0x1) {
            ++addr;
        }
        switch (range & 0x3) {
            case 0x0: // space
            case 0x3: // 1x1
                if (addr > 19) {
                    return false;
                }
                status |= 0x1 << addr;
                break;
            case 0x1: // 1x2
                if (addr % 4 == 3 || addr > 18 || status >> (addr + 1) & 0x1) {
                    return false;
                }
                status |= 0x3 << addr;
                break;
            case 0x2: // 2x1
                if (addr > 15 || status >> (addr + 4) & 0x1) {
                    return false;
                }
                status |= 0x11 << addr;
                break;
        }
    }
    return true;
}
