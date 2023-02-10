#include "gtest/gtest.h"
#include "common_code.h"
#include "global_utils.h"

using klotski::CommonCode;

std::vector<uint64_t> common_code_search(uint64_t start, uint64_t end) {
    std::vector<uint64_t> ret;
    for (uint64_t common_code = start; common_code < end; ++common_code) {
        if (CommonCode::check(common_code)) {
            ret.emplace_back(common_code); // valid common code
        }
    }
    return ret;
}


TEST(GLOBAL, common_code) {

//    auto ranges = range_split(0, 100, 30);
//    auto ranges = range_split(0, 91, 30);
//    auto ranges = range_split(0, 89, 30);
    auto ranges = range_split(0, 90, 30);

    for (const auto &r : ranges) {
        printf("[%lu, %lu)\n", r.first, r.second);
    }

}
