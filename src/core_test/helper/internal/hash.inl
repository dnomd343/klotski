#pragma once

namespace helper {

inline uint64_t xxh3(const std::string_view &data) {
    return xxh3(data.data(), data.size());
}

inline std::string md5(const std::string_view &data) {
    return md5(data.data(), data.size());
}

template <typename T>
requires std::is_trivial_v<T>
uint64_t xxh3(const std::vector<T> &data) {
    return xxh3(data.data(), data.size() * sizeof(T));
}

template <typename T>
requires std::is_trivial_v<T>
std::string md5(const std::vector<T> &data) {
    return md5(data.data(), data.size() * sizeof(T));
}

} // namespace helper
