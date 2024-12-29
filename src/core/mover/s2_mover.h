#pragma once

#include <functional>

namespace klotski::mover {

class S2Mover {
public:
    using release_t = std::function<void(uint64_t)>;

    explicit S2Mover(release_t release_func) : release_(std::move(release_func)) {}

    void next_cases(uint64_t code);

private:
    release_t release_;

    void two_space_a(uint64_t code, int offset) const;
    void two_space_b(uint64_t code, int offset) const;

    void one_space(uint64_t code, int offset) const;
};

} // namespace klotski::mover