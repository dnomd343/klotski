#pragma once

#include <ostream>

class Benchmark {
public:
    static void all_cases(std::ostream &out);
    static void basic_ranges(std::ostream &out);

private:
    static std::string time_s(clock_t start);
    static std::string time_ms(clock_t start);
    static std::string time_us(clock_t start);
    static std::string time_ns(clock_t start);

    static std::string color_red(const std::string &str);
    static std::string color_blue(const std::string &str);
    static std::string color_green(const std::string &str);
    static std::string color_yellow(const std::string &str);
};
