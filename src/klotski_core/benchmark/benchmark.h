#pragma once

#include <ostream>

namespace klotski {
    class Benchmark {
    public:
        enum TIME {
            S, MS, US, NS
        };
        static float all_cases(enum TIME format);
        static float basic_ranges(enum TIME format);

    private:
//        static std::string time_s(clock_t start);
//        static std::string time_ms(clock_t start);
//        static std::string time_us(clock_t start);
//        static std::string time_ns(clock_t start);
        static float time_format(clock_t start, enum TIME format);
    };
}
