#include "gtest/gtest.h"
#include "short_code.h"

using klotski::ShortCode;
using klotski::CommonCode;

std::vector<uint64_t> short_code_check(uint32_t start, uint32_t end) {

    // TODO: give me short_code range -> convert to common_code and archive it
    //       common_code convert to short_code -> verify normal mode

    std::vector<uint64_t> archive;

    for (uint32_t short_code = start; short_code < end; ++short_code) {

        auto common_code = ShortCode::unsafe_create(short_code).to_common_code();

        archive.emplace_back(common_code.unwrap());

        EXPECT_EQ(common_code.to_short_code().unwrap(), short_code);

    }

    return archive;

}

TEST(GLOBAL, short_code) {

    ShortCode::speed_up(ShortCode::NORMAL);

    auto r = short_code_check(0, 100);

    std::cout << r.size() << std::endl;


}
