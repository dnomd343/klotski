#pragma once

#include <ostream>

class Benchmark {
public:


    static void basic_ranges(std::ostream &out);

    static inline void time_ms(clock_t start, std::ostream &out);
    static inline void time_us(clock_t start, std::ostream &out);

    static inline long time_ms_(clock_t start);

    static inline std::string time_ms_str(clock_t start);

};
