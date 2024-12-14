#include "helper/mirror.h"

#include "utils/common.h"

using klotski::codec::RawCode;
using klotski::codec::CommonCode;

using namespace klotski;

RawCode helper::get_horizontal_mirror(RawCode code) {
    uint64_t new_code = 0;

    for (int addr = 0; addr < 20; ++addr) {

        auto block = (code.unwrap() >> (addr * 3)) & 0b111;

        if (block == BLOCK_CE_1x1) {
            auto target_addr = addr - (addr % 4);
            target_addr += 3 - (addr % 4);
            new_code |= K_MASK_1x1 << (target_addr * 3);
        } else if (block == BLOCK_CE_2x1) {
            auto target_addr = addr - (addr % 4);
            target_addr += 3 - (addr % 4);
            new_code |= K_MASK_2x1 << (target_addr * 3);
        } else if (block == BLOCK_CE_1x2) {
            auto target_addr = addr - (addr % 4);
            if (addr % 4 == 0) {
                target_addr += 2;
            } else if (addr % 4 == 1) {
                target_addr += 1;
            }
            new_code |= K_MASK_1x2 << (target_addr * 3);
        } else if (block == BLOCK_CE_2x2) {
            auto target_addr = addr - (addr % 4);
            if (addr % 4 == 0) {
                target_addr += 2;
            } else if (addr % 4 == 1) {
                target_addr += 1;
            }
            new_code |= K_MASK_2x2 << (target_addr * 3);
        }

    }

    return RawCode::unsafe_create(new_code);
}

RawCode helper::get_vertical_mirror(RawCode code) {
    uint64_t new_code = 0;

    for (int addr = 0; addr < 20; ++addr) {

        auto block = (code.unwrap() >> (addr * 3)) & 0b111;

        if (block == BLOCK_CE_1x1) {
            auto target_addr = addr - (addr % 4);
            target_addr = 16 - target_addr;
            target_addr += (addr % 4);
            new_code |= K_MASK_1x1 << (target_addr * 3);
        } else if (block == BLOCK_CE_2x1) {
            auto target_addr = addr - (addr % 4);
            target_addr = 12 - target_addr;
            target_addr += (addr % 4);
            new_code |= K_MASK_2x1 << (target_addr * 3);
        } else if (block == BLOCK_CE_1x2) {
            auto target_addr = addr - (addr % 4);
            target_addr = 16 - target_addr;
            target_addr += (addr % 4);
            new_code |= K_MASK_1x2 << (target_addr * 3);
        } else if (block == BLOCK_CE_2x2) {
            auto target_addr = addr - (addr % 4);
            target_addr = 12 - target_addr;
            target_addr += (addr % 4);
            new_code |= K_MASK_2x2 << (target_addr * 3);
        }

    }

    return RawCode::unsafe_create(new_code);
}

CommonCode helper::get_vertical_mirror(CommonCode code) {
    return get_vertical_mirror(code.to_raw_code()).to_common_code();
}

CommonCode helper::get_horizontal_mirror(CommonCode code) {
    return get_horizontal_mirror(code.to_raw_code()).to_common_code();
}
