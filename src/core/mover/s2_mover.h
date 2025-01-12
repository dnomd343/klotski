#pragma once

#include <functional>

#include "raw_code/raw_code_fwd.h"

namespace klotski::mover {

class S2Mover {
public:
    using release_t = std::function<void(codec::RawCode)>;

    explicit S2Mover(release_t release_func) : release_(std::move(release_func)) {}

    void next_cases(codec::RawCode code) const;

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

class S2HintMover {
public:
    using release_t = std::function<void(codec::RawCode, uint64_t)>;

    explicit S2HintMover(release_t release_func) : release_(std::move(release_func)) {}

    void next_cases(codec::RawCode code, uint64_t hint) const;

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
