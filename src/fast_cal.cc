#include <iostream>
#include <unordered_map>
#include <functional>
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


class FastCal;
class GlobalCal;

//template<typename T>
class CoreDemo {
public:
//    typedef void (T::*release_t)(uint64_t);

    std::function<void(uint64_t)> release;

//    void next(uint64_t code, T *f, release_t release_func) {
    void next(uint64_t code) {

        std::cout << "Core get code = " << code << std::endl;
        std::cout << "Core callback first time" << std::endl;
//        (f->*release_func)(++code);
        release(++code);
        std::cout << "Core callback second time" << std::endl;
//        (f->*release_func)(++code);
        release(++code);
        std::cout << "Core function exit" << std::endl;

    }

};

class FastCal {
public:
    uint64_t data;

    explicit FastCal(uint64_t dat) : data(dat) {}

    void callback(uint64_t code) {
        std::cout << "FastCal get callback " << code << std::endl;
        data = code;
        std::cout << "FastCal set data " << data << std::endl;
    }

    void run() {
//        auto cd = CoreDemo<FastCal>();
        auto cd = CoreDemo();
        cd.release = std::bind(&FastCal::callback, this, std::placeholders::_1);

        std::cout << "FastCal data = " << data << std::endl;

//        cd.next(data, this, &FastCal::callback);
        cd.next(data);
    }

};

class GlobalCal {
public:
    uint64_t data;

    explicit GlobalCal(uint64_t dat) : data(dat) {}

    void callback(uint64_t code) {
        std::cout << "GlobalCal get callback " << code << std::endl;
        data = code;
        std::cout << "GlobalCal set data " << data << std::endl;
    }

    void run() {
//        auto cd = CoreDemo<GlobalCal>();
        auto cd = CoreDemo();
        cd.release = std::bind(&GlobalCal::callback, this, std::placeholders::_1);

        std::cout << "GlobalCal data = " << data << std::endl;

//        cd.next(data, this, &GlobalCal::callback);
        cd.next(data);
    }

};


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

    auto f1 = FastCal(1000);
    auto f2 = FastCal(2000);
    auto g = GlobalCal(3000);

    f2.run();
    f1.run();
    f1.run();
    g.run();

//    auto g = GlobalCal();
//    g.run();

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
