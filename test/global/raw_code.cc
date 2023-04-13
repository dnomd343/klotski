#include "common.h"
#include "raw_code.h"
#include "gtest/gtest.h"
#include "global_utils.h"

using klotski::RawCode;
using klotski::AllCases;
using klotski::CommonCode;
using klotski::Common::range_reverse;

uint64_t convert(uint64_t common_code) { // try to convert as raw code
    auto code = C_2x2 << (common_code >> 32) * 3;
    auto range = range_reverse((uint32_t)common_code);
    for (int addr = 0; range; range >>= 2) {
        while ((code >> addr) & 0b111 && addr < 60) {
            addr += 3;
        }
        if (addr >= 60) {
            return 0;
        }
        switch (range & 0b11) { // match low 2-bits
            case 0b01: // 1x2 block
                code |= C_1x2 << addr;
                break;
            case 0b10: // 2x1 block
                code |= C_2x1 << addr;
                break;
            case 0b11: // 1x1 block
                code |= C_1x1 << addr;
                break;
            case 0b00: // space
                addr += 3;
        }
    }
    return code;
}

std::vector<uint64_t> raw_code_search(uint64_t start, uint64_t end) {
    std::vector<uint64_t> ret;
    for (uint64_t common_code = start; common_code < end; ++common_code) {
        if (RawCode::check(convert(common_code))) {
            ret.emplace_back(common_code); // valid layout
        }
    }
    return ret;
}

TEST(GLOBAL, raw_code) {
    /// create raw code check tasks
    auto pool = TinyPool();
    std::vector<std::future<std::vector<uint64_t>>> futures;
    for (const auto &range : range_split(0, 0x10'0000'0000, 0x10'0000)) {
        futures.emplace_back(
            pool.submit(raw_code_search, range.first, range.second)
        );
    }

    /// run raw code search
    pool.boot();
    std::vector<uint64_t> result;
    for (auto &f : futures) {
        auto ret = f.get();
        result.insert(result.end(), ret.begin(), ret.end());
    }
    pool.join();

    /// verify check result
    std::vector<uint64_t> all_cases;
    for (uint64_t head = 0; head < 16; ++head) {
        for (const auto &range : AllCases::fetch()[head]) {
            all_cases.emplace_back(head << 32 | range);
        }
    }
    EXPECT_EQ(result, all_cases);
}
