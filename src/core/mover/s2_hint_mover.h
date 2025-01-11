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
