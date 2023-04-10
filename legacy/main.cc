#include <iostream>

#include "ffi/klotski.h"

int main() {

//    for (const auto &c : FastCal::resolve(RawCode::from_common_code("1a9bf0c"))) {
//        std::cout << c << std::endl;
//    }
//
//    auto a = Analyse(RawCode::from_common_code("1a9bf0c"));
//    for (const auto &s : a.build_resolve()) {
//        std::cout << s << std::endl;
//    }

    return 0;


//    auto fc = FastCal(RawCode::from_common_code("1a9bf0c"));
//    fc.solve();

//    {
//        auto fc = FastCal(RawCode::unsafe_create(0));
//        for (auto code : test_cases) {
//            fc.set_root(code);
//            fc.solve();
//        }
//    }


//    auto a = Analyse(RawCode::from_common_code("1a9bf0c"));
//    auto a = Analyse(RawCode::from_common_code("4fea134"));
//    auto a = Analyse(RawCode::from_common_code("A5D3AF"));


//    a.build();
//    auto ret = a.build_until([](uint64_t code) {
//        return ((code >> (3 * 0xD)) & 0b111) == B_2x2;
//    });
//    auto ret = a.build_resolve();
//    for (const auto &r : ret) {
//        std::cout << r << std::endl;
//    }
//    std::cout << ret.size() << std::endl;

//    start_time = clock();
//    auto svg_ret = a.backtrack(ret);
//    std::cerr << (clock() - start_time) * 1000000 / CLOCKS_PER_SEC << "us" << std::endl;


//    auto g = Graph();
//    auto svg_data = g.svg_demo(svg_ret);

//    std::cout << svg_data << std::endl;

//    std::cout << a.layer_export(81).size() << std::endl;
//    std::cout << a.layer_export()[81].size() << std::endl;


//    std::vector<uint64_t> raw_codes = {
//        CommonCode(0xDAAF4CC00).to_raw_code().unwrap(),
//        CommonCode(0xDAA7F3000).to_raw_code().unwrap(),
//    };
//    a.backtrack(raw_codes);

//    std::vector<uint64_t> raw_codes = {
//        CommonCode("DE77D80").to_raw_code().unwrap(),
//        CommonCode("DF6DD80").to_raw_code().unwrap(),
//        CommonCode("DF69730").to_raw_code().unwrap(),
//        CommonCode("DE76D30").to_raw_code().unwrap(),
//        CommonCode("D5AF730").to_raw_code().unwrap(),
//        CommonCode("D5B43BC").to_raw_code().unwrap(),
//    };
//    std::vector<uint64_t> raw_codes = {
//        CommonCode("EF697C0").to_raw_code().unwrap(),
//    };
    // TODO: FATAL ERROR -> don't using mask in Analyse
//    a.backtrack(raw_codes);

    return 0;
}
