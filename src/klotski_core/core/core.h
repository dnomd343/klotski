#pragma once

/// The main purpose of klotski calculation is to obtain all the next steps in order to carry
/// out the BFS algorithm. The definition of the next step is the result of moving an arbitrary
/// block in the current layout for a limited number of times, and the resulting layout is
/// different from the original.

///   Eg1:
///     % # # %       % # # %    % # # %    % # # %    % # # %    % # # %    % # # %    % # # %    % # # %
///     % # # %       % # # %    % # # %    % # # %    % # # %    % # # %    % # # %    % # # %    % # # %
///     @ $ $ @  ==>  @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @
///     @ & * @       @   * @    @   * @    @ &   @    @ &   @    @ & * @    @ & * @    @ & * @    @ & * @
///     *     &       * &   &    *   & &    *   * &    * *   &      *   &        * &    *   &      * &

///   Eg2:
///     * @ & %       * @ & %    * @ & %    * @ & %
///     # # $ %       # # $ %    # # $ %    # # $ %
///     # # $ ^  ==>  # # $ ^    # # $ ^    # # $ ^
///       ~ ~ ^       ~ ~   ^      ~ ~ ^    @ ~ ~ ^
///       @ % %         @ % %    @   % %        % %

/// In order to achieve the purpose of searching for the next step, we should find out the
/// sub-layout that can be derived from each block. Due to the limited steps of moving, with
/// the help of a sub-BFS search, the target can be obtained. It can be shown that this step
/// produces at most 15 derived layouts, so it can be stored and computed in a queue of length
/// 16. By performing such a search on each block in the layout, we can get all the next-step
/// layouts, which have a minimum of 0 and a maximum of 68.

#include <cstdint>
#include <utility>
#include <functional>

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
