/// Klotski Engine by Dnomd343 @2024

// TODO: only copy from old implementation, the interfaces will change in future.

/// The main purpose of klotski calculation is to obtain all the next steps in
/// order to carry out the BFS algorithm. The definition of the next step is
/// the result of moving an arbitrary block in the current layout for a limited
/// number of times, and the resulting layout is different from the original.

///   Eg1:
///     % # # %       % # # %    % # # %    % # # %    % # # %
///     % # # %       % # # %    % # # %    % # # %    % # # %
///     @ $ $ @  -->  @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @
///     @ & * @       @   * @    @   * @    @ &   @    @ &   @
///     *     &       * &   &    *   & &    *   * &    * *   &
///        |
///        |          % # # %    % # # %    % # # %    % # # %
///        |          % # # %    % # # %    % # # %    % # # %
///        | ------>  @ $ $ @    @ $ $ @    @ $ $ @    @ $ $ @
///                   @ & * @    @ & * @    @ & * @    @ & * @
///                     *   &        * &    *   &      * &

///   Eg2:
///     * @ & %       * @ & %    * @ & %    * @ & %
///     # # $ %       # # $ %    # # $ %    # # $ %
///     # # $ ^  -->  # # $ ^    # # $ ^    # # $ ^
///       ~ ~ ^       ~ ~   ^      ~ ~ ^    @ ~ ~ ^
///       @ % %         @ % %    @   % %        % %

/// In order to achieve the purpose of searching for the next step, we should
/// find out the sub-layouts that can be derived from each block. Due to the
/// limited steps of moving, with the help of a sub-BFS search, the target can
/// be obtained. It can be shown that this step produces at most `15` derived
/// layouts, so it can be stored and computed in a queue which length 16. By
/// performing such a search on each block in the layout, we can get all the
/// next-step layouts, which have a minimum of `0` and a maximum of `68`.

/// For a single block, search for the situation after it has moved one grid,
/// `addr` is its current position information, which is (0 ~ 19) * 3. When
/// moving up and down, judge the value of `addr` to confirm whether it is out
/// of bounds; when moving left and right, get its remainder to `4` to judge
/// whether it will be out of bounds. After confirming that it will not cross
/// the boundary, it is necessary to judge whether it will collide with other
/// blocks after moving, that is, the target position needs to be empty, which
/// is represented as `000` in the RawCode, and the template will be used to
/// perform bit operations here to confirm whether it is feasible.

/// Finally, in order to improve efficiency, the `filter` option is added. For
/// the direction of the last movement, it will be ignored in the next search,
/// so as to optimize BFS. After confirming that it can be moved, the moved
/// layout can be directly obtained by means of bit operations, and a mask will
/// be obtained at the same time, which marks the moved block as `111`, which
/// will speed up subsequent calculations. The generated layout will be
/// inserted into the cache, and after the BFS search for each block is
/// completed, the mover will use the callback function to output these
/// results.

#pragma once

#include <cstdint>
#include <utility>
#include <functional>

#include "raw_code/raw_code.h"

namespace klotski::mover {

// TODO: new version without mask

// TODO: allow wrap as a function directly

class MaskMover {
public:
    /// Release with code and mask
    typedef std::function<void(codec::RawCode, uint64_t)> release_t;

    /// Core interface
    void next_cases(codec::RawCode code, uint64_t mask);
    explicit MaskMover(release_t release_func) : release_(std::move(release_func)) {}

private:
    struct cache_t {
        uint64_t code;
        uint64_t mask; /// (000) or (111)
        int filter; /// DIR_UP | DIR_DOWN | DIR_LEFT | DIR_RIGHT
        int addr; /// (0 ~ 19) * 3
    };

    int cache_size_ = 1;
    cache_t cache_[16]{};
    release_t release_; // release function

    void move_1x1(uint64_t code, int addr);
    void move_1x2(uint64_t code, int addr);
    void move_2x1(uint64_t code, int addr);
    void move_2x2(uint64_t code, int addr);
    inline void cache_insert(cache_t next_case);
};

} // namespace klotski::mover
