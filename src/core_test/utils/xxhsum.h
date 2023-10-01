#pragma once

#include <string>
#include "xxhash.h"

namespace xxhash {

std::string xxhsum(const void *data, size_t size) {
    char *hash;
    auto state = XXH64_createState();
    XXH64_reset(state, 0);
    XXH64_update(state, data, size);
    asprintf(&hash, "%016llx", XXH64_digest(state));
    XXH64_freeState(state);
    return hash;
}

std::string xxhsum(const std::string &data) {
    return xxhsum(data.c_str(), data.length());
}

template <typename T>
std::string xxhsum(const std::vector<T> &data) {
    return xxhsum(data.data(), data.size() * sizeof(T));
}

} // namespace xxhsum
