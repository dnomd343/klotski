#pragma once

#include <cstdint>
#include <utility>
#include <functional>

#define UP    (-4 * 3)
#define LEFT  (-1 * 3)
#define DOWN  (+4 * 3)
#define RIGHT (+1 * 3)

class Core {
public:
    /// Release with code and mask
    typedef std::function<void(uint64_t, uint64_t)> release_t;

    /// Core interface
    void next_cases(uint64_t code, uint64_t mask);
    explicit Core(release_t release_func) : release(std::move(release_func)) {}

private:
    struct cache_t {
        uint64_t code;
        uint64_t mask; /// 000 or 111
        int filter; /// UP | DOWN | LEFT | RIGHT
        int addr; /// (0 ~ 19) * 3
    };

    int cache_size = 1;
    cache_t cache[16]{};
    release_t release; // release function

    void move_1x1(uint64_t code, int addr);
    void move_1x2(uint64_t code, int addr);
    void move_2x1(uint64_t code, int addr);
    void move_2x2(uint64_t code, int addr);
    inline void cache_insert(Core::cache_t next_case);
};
