#pragma once

#include <string>
#include <gtest/gtest.h>

// ----------------------------------------------------------------------------------------- //

/// Capture ostream output as string.
template <typename T>
std::string ostream_capture(T obj) {
    std::ostringstream out;
    out << obj; // ostream capture
    return out.str();
}

#define EXPECT_OSTREAM(obj, expect) EXPECT_EQ(ostream_capture(obj), expect)

// ----------------------------------------------------------------------------------------- //
