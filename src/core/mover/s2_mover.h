#pragma once

#include <functional>

namespace klotski::mover {

class S2Mover {
public:
    using release_t = std::function<void(uint64_t)>;

    explicit S2Mover(release_t release_func) : release_(std::move(release_func)) {}

    void next_cases(uint64_t code) const;

private:
    release_t release_;

    template <int N>
    void move_single(uint64_t code) const;

    template <int N>
    void move_double_h(uint64_t code) const;

    template <int N>
    void move_double_v(uint64_t code) const;

    void move_single(uint64_t code, int offset) const;
    void move_double_h(uint64_t code, int offset) const;
    void move_double_v(uint64_t code, int offset) const;
};

} // namespace klotski::mover
