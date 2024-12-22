/// Klotski Engine Python FFI by Dnomd343 @2024

#pragma once

namespace klotski::ffi {

// TODO: add multi-thread support

// TODO: add `__await__` interface support

class PySpeedUp {
public:
    static void stage_0();

    static void stage_1();

    static void stage_2();

    static bool is_stage_0();

    static bool is_stage_1();

    static bool is_stage_2();
};

} // namespace klotski::ffi
