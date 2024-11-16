#pragma once

/// Provides XXH3 and MD5 hash calculation support. The former is far ahead in
/// speed, but the latter is more of a commemorative significance.

/// They do not guarantee the security of the hash algorithm, as this scenario
/// is only used to verify the correctness of the data.

#include <string>
#include <vector>
#include <cstdint>
#include <string_view>

namespace helper {

// ----------------------------------------------------------------------------------------- //

/// Calculate XXH3 hash value as u64.
uint64_t xxh3(const std::string_view &data);

/// Calculate XXH3 hash value as u64.
uint64_t xxh3(const void *data, uint64_t size);

/// Calculate MD5 hash value as string.
std::string md5(const std::string_view &data);

/// Calculate MD5 hash value as string.
std::string md5(const void *data, uint64_t size);

// ----------------------------------------------------------------------------------------- //

/// Calculate XXH3 hash value of continuous array.
template <typename T>
requires std::is_trivial_v<T>
uint64_t xxh3(const std::vector<T> &data);

/// Calculate MD5 hash value of continuous array.
template <typename T>
requires std::is_trivial_v<T>
std::string md5(const std::vector<T> &data);

// ----------------------------------------------------------------------------------------- //

} // namespace helper

#include "internal/hash.inl"
