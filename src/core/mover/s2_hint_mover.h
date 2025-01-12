#pragma once

#include <functional>

namespace klotski::mover {

class S2HintMover {
public:
    using release_t = std::function<void(uint64_t, uint64_t)>;

    explicit S2HintMover(release_t release_func) : release_(std::move(release_func)) {}

    void next_cases(uint64_t code, uint64_t hint) const;

private:
    release_t release_;

    template <int N>
    void move_single(uint64_t code, uint64_t hint) const;

    template <int N>
    void move_double_h(uint64_t code, uint64_t hint) const;

    template <int N>
    void move_double_v(uint64_t code, uint64_t hint) const;

    void move_single(uint64_t code, uint64_t hint, int offset) const;
    void move_double_h(uint64_t code, uint64_t hint, int offset) const;
    void move_double_v(uint64_t code, uint64_t hint, int offset) const;
};

} // namespace klotski::mover

namespace klotski::mover {

// TODO: It seems that storage space in `hint` will cause performance
//       degradation, maybe remove this optimization.

class S2HintMoverPro {
public:
    using release_t = std::function<void(uint64_t, uint64_t)>;

    explicit S2HintMoverPro(release_t release_func) : release_(std::move(release_func)) {}

    void next_cases(uint64_t code, uint64_t hint) const;

    static uint64_t make_hint(uint64_t code);

private:
    release_t release_;

    template <int N>
    void move_single(uint64_t code, uint32_t hint, uint32_t kk) const;

    template <int N>
    void move_double_h(uint64_t code, uint32_t hint) const;

    template <int N>
    void move_double_v(uint64_t code, uint32_t hint) const;

    void move_single(uint64_t code, uint32_t hint, int offset, int offset_) const;
    void move_double_h(uint64_t code, uint32_t hint, int offset) const;
    void move_double_v(uint64_t code, uint32_t hint, int offset) const;
};

} // namespace klotski::mover
