#include "analyse/analyse.h"

#include <print>

using klotski::codec::RawCode;
using klotski::mover::MaskMover;
using klotski::analyse::AnalysePro;

void AnalysePro::build_all() {
    auto mover = MaskMover([this](const RawCode code, const uint64_t mask) {
        try_emplace(code, mask);
    });

    while (!seeker_.is_ending()) {
        // std::println("layer: {}", seeker_.layer_num());
        spawn_next(mover);
    }
}
