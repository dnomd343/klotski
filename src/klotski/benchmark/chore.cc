#include "benchmark.h"

using namespace klotski;

// TODO: remove std::cout output -> only return benchmark status

/// colorful string
//std::string Benchmark::color_red(const std::string &str) {
//    return std::string("\033[31m") + str + "\033[0m";
//}
//
//std::string Benchmark::color_blue(const std::string &str) {
//    return std::string("\033[36m") + str + "\033[0m";
//}
//
//std::string Benchmark::color_green(const std::string &str) {
//    return std::string("\033[32m") + str + "\033[0m";
//}
//
//std::string Benchmark::color_yellow(const std::string &str) {
//    return std::string("\033[33m") + str + "\033[0m";
//}

/// used-time to green string
//std::string Benchmark::time_s(clock_t start) {
//    return color_green(
//        std::to_string((clock() - start) / CLOCKS_PER_SEC) + "s"
//    );
//}
//
//std::string Benchmark::time_ms(clock_t start) {
//    return color_green(
//        std::to_string((clock() - start) * 1000 / CLOCKS_PER_SEC) + "ms"
//    );
//}
//
//std::string Benchmark::time_us(clock_t start) {
//    return color_green(
//        std::to_string((clock() - start) * 1000 * 1000 / CLOCKS_PER_SEC) + "us"
//    );
//}
//
//std::string Benchmark::time_ns(clock_t start) {
//    return color_green(
//        std::to_string((clock() - start) * 1000 * 1000 * 1000 / CLOCKS_PER_SEC) + "us"
//    );
//}

float Benchmark::time_format(clock_t start, Benchmark::TIME format) {
    auto time = float(clock() - start);
    switch (format) {
        case S:
            time *= float(1);
            break;
        case MS:
            time *= float(1000);
            break;
        case US:
            time *= float(1000 * 1000);
            break;
        case NS:
            time *= float(1000 * 1000 * 1000);
            break;
    }
    return time / CLOCKS_PER_SEC;
}
