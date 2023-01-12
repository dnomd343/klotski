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
    fast_cal_t *last;
};

std::queue<fast_cal_t*> cache;

std::unordered_map<uint64_t, fast_cal_t> cases;

void add_global_demo(uint64_t code) {
    std::cout << RawCode(code).dump_case() << std::endl;
}

class FastCal;
class GlobalCal;

template<typename T>
class CoreDemo {
public:
    int code{0};

    typedef void (T::*release_t)(uint64_t);

//    void next(release_t release_func) {
//        release_func(1);
//        release_func(2);
//        release_func(3);
//        ++code;
//    }

    void next(T *f, release_t release_func) {

        (f->*release_func)(1);

//        release_func(2);
//        release_func(3);
        ++code;
    }

};


class FastCal {
public:
    int dat;

    void add(uint64_t code) {
        std::cout << RawCode(code).dump_case() << std::endl;
        ++dat;
    }

    void run() {
        auto cd = CoreDemo<FastCal>();

//        cd.next(add_global_demo);
//        cd.next(this->*add);

        cd.next(this, &FastCal::add);

        ++dat;
    }

};


class GlobalCal {
public:
    int dat;

    void add(uint64_t code) {
        std::cout << RawCode(code).dump_case() << std::endl;
        ++dat;
    }

    void run() {
        auto cd = CoreDemo<GlobalCal>();

        cd.next(this, &GlobalCal::add);

        ++dat;
    }

};

//void (FastCal::*ptrStaticFun)(uint64_t) = &FastCal::add;
//release_t ptrFun = &FastCal::add;

//void demo() {
//    auto f = new FastCal();
//    (f->*ptrFun)(1);
//}

void add_new_case(uint64_t code, uint64_t mask) {

    auto exist_case = cases.find(code);
    if (exist_case != cases.end()) { // find existed case

        exist_case->second.mask |= mask; // mask update
        return;

    }

    cases[code] = fast_cal_t {
        .code = code,
        .mask = mask,
        .last = cache.front(),
    };;
    cache.emplace(&cases[code]);

    // TODO: try to check head address = 0xD at here

//    if (((code >> (3 * 0xD)) & 0b111) == B_2x2) {
//        std::cout << "Resolved" << std::endl;
//            std::cout << RawCode(cache.front()->code).dump_case() << std::endl;
//        stop_flag = true;
//    }

}

uint32_t fast_cal(uint64_t code) {

    auto f = FastCal();
    f.run();

    auto g = GlobalCal();
    g.run();

    return 0;


    auto core = Core(add_new_case);

    cases.empty();

    cache.empty();

    cases[code] = fast_cal_t {
        .code = code,
        .mask = 0,
        .last = nullptr,
    };
    cache.emplace(&cases[code]);

    while (!cache.empty()) {

        // break check point
        if (((cache.front()->code >> (3 * 0xD)) & 0b111) == B_2x2) {
            std::cout << "Resolved" << std::endl;
//            std::cout << RawCode(cache.front()->code).dump_case() << std::endl;
            break;
        }

//        core.next_step(cache.front()->code, cache.front()->mask);
        cache.pop();
    }

    auto solution = cache.front();

    while (solution != nullptr) {
        std::cout << RawCode(solution->code).dump_case() << std::endl;
        solution = solution->last;
    }

    return cases.size();

}
