#pragma once

/// This is a built-in performance testing module for klotski core, which will
/// directly call the underlying functions to avoid the additional overhead
/// of the glue ABI.

/// Multiple test items are provided here, and the corresponding float value is
/// returned according to the specified time format (seconds, milliseconds,
/// microseconds, nanoseconds).

/// The test items are all multi-thread safe, but you should not run multiple
/// test items at the same time, which will lead to unstable tests in many ways,
/// such as changes in CPU turbo frequency.

/// Pay attention to the two test items `basic_ranges` and `all_cases`, they can
/// only be run once (the reason for the construction of static data), and cannot
/// be run after other global related items.

#include <ctime>
#include <mutex>
#include <string>
#include <vector>
#include <cstdint>

#include "raw_code.h"
#include "short_code.h"
#include "common_code.h"

namespace klotski {

class Benchmark {
public:
    enum TIME {
        S, MS, US, NS
    };
    static void data_initialize() noexcept;
    static double warm_up(uint64_t count) noexcept;
    static double range_flip(TIME format = NS) noexcept;

    static double all_cases(TIME format = MS) noexcept;
    static double basic_ranges(TIME format = MS) noexcept;

    static double raw_code_check(TIME format = NS) noexcept;
    static double short_code_check(TIME format = NS) noexcept;
    static double common_code_check(TIME format = NS) noexcept;
    static double raw_code_check_random(TIME format = NS) noexcept;
    static double short_code_check_random(TIME format = NS) noexcept;
    static double common_code_check_random(TIME format = NS) noexcept;

    static double short_code_to_string(TIME format = NS) noexcept;
    static double short_code_from_string(TIME format = NS) noexcept;
    static double common_code_to_string(TIME format = NS) noexcept;
    static double common_code_from_string(TIME format = NS) noexcept;

    static double common_code_to_raw_code(TIME format = NS) noexcept;
    static double raw_code_to_common_code(TIME format = NS) noexcept;
    static double common_code_to_short_code(TIME format = US) noexcept;
    static double short_code_to_common_code(TIME format = US) noexcept;
    static double common_code_to_short_code_fast(TIME format = NS) noexcept;
    static double short_code_to_common_code_fast(TIME format = NS) noexcept;

    static double vertical_mirror_check(TIME format = NS) noexcept;
    static double horizontal_mirror_check(TIME format = NS) noexcept;
    static double vertical_mirror_convert(TIME format = NS) noexcept;
    static double horizontal_mirror_convert(TIME format = NS) noexcept;

private:
    static bool data_ready_;
    static std::mutex data_building_;
    static std::vector<RawCode> all_raw_codes_;
    static std::vector<ShortCode> all_short_codes_;
    static std::vector<CommonCode> all_common_codes_;
    static std::vector<std::string> all_short_codes_str_;
    static std::vector<std::string> all_common_codes_str_;

    static void build_data() noexcept;
    static uint32_t random_seed() noexcept;
    static double time_format(clock_t start, TIME format) noexcept;
    static std::vector<uint32_t> generate_u32_rand(uint32_t count) noexcept;
    static std::vector<uint64_t> generate_u64_rand(uint32_t count) noexcept;
};

} // namespace klotski
