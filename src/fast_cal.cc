#include <iostream>
#include <unordered_map>
#include <queue>
#include "core.h"
#include "fast_cal.h"
#include "raw_code.h"
#include "common.h"

struct fast_cal_t {
    uint64_t code;
    uint64_t mask;
//    uint32_t step;
};

std::unordered_map<uint64_t, fast_cal_t> cal_data;
std::queue<fast_cal_t*> cal_temp;

void add_new_case(uint64_t code, uint64_t mask) {

//    std::cout << RawCode(code).dump_case();
//    std::cout << "~~~~~~~" << std::endl;
//    auto temp_mask = mask;
//    for (int n = 0; n < 20; ++n, temp_mask >>= 3) {
//        std::cout << "+."[!(temp_mask & 0b111)] << " \n"[!(~n & 0b11)];
//    }
//    std::cout << std::endl;

    auto exist_case = cal_data.find(code);
    if (exist_case != cal_data.end()) { // find it

        exist_case->second.mask |= mask; // mask update

        return;
    }

//    std::cout << RawCode(code).dump_case();
//    std::cout << "~~~~~~~" << std::endl;
//    auto temp_mask = mask;
//    for (int n = 0; n < 20; ++n, temp_mask >>= 3) {
//        std::cout << "+."[!(temp_mask & 0b111)] << " \n"[!(~n & 0b11)];
//    }
//    std::cout << std::endl;

    auto new_case = fast_cal_t {
        .code = code,
        .mask = mask,
    };

    cal_data[code] = new_case;

    // TODO: avoid redundancy map search
    cal_temp.emplace(&cal_data[code]);

}

uint32_t fast_cal(uint64_t start_raw_code) {
//    std::cout << RawCode(start_raw_code).dump_case() << std::endl;

    auto core = Core(add_new_case);

    cal_data.empty();

    auto setup = fast_cal_t {
        .code = start_raw_code,
        .mask = 0,
    };

    cal_data[start_raw_code] = setup;
    cal_temp.emplace(&cal_data[start_raw_code]);

    while (!cal_temp.empty()) {

        if (((cal_temp.front()->code >> (3 * 0xD)) & 0b111) == B_2x2) {
            std::cout << "get resolve" << std::endl;
            std::cout << RawCode(cal_temp.front()->code).dump_case() << std::endl;
            break;
        }

        core.next_step(cal_temp.front()->code, cal_temp.front()->mask);
        cal_temp.pop();
    }

//    core.next_step(cal_temp.front()->code, cal_temp.front()->mask);
//    cal_temp.pop();
//
//    core.next_step(cal_temp.front()->code, cal_temp.front()->mask);
//    cal_temp.pop();

//    std::cout << "size = " << cal_data.size() << std::endl;
//    std::cout << "queue size = " << cal_temp.size() << std::endl;

    return cal_data.size();

}
