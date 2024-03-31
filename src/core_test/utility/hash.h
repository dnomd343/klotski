#pragma once

/// Provides XXH3 and MD5 hash calculation support. The former is far ahead in
/// speed, but the latter is more of a commemorative significance.

#include "md5.h"
#include "xxh3.h"

#include <vector>
#include <cstdint>

namespace hash {

inline std::string md5(const void *data, const uint64_t size) {
    return ::md5::MD5::Hash(data, size);
}

inline uint64_t xxh3(const void *data, const uint64_t size) {
    return XXH_INLINE_XXH3_64bits(data, size);
}

inline std::string md5(const std::string_view &data) {
    return md5(data.data(), data.size());
}

inline uint64_t xxh3(const std::string_view &data) {
    return xxh3(data.data(), data.size());
}

template <typename T>
inline std::string md5(const std::vector<T> &data) {
    return md5(data.data(), data.size() * sizeof(T));
}

template <typename T>
inline uint64_t xxh3(const std::vector<T> &data) {
    return xxh3(data.data(), data.size() * sizeof(T));
}

} // namespace hash
