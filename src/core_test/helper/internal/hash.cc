#include "helper/hash.h"

#include <md5.h>
#include <xxh3.h>

std::string helper::md5(const void *data, const uint64_t size) {
    return md5::MD5::Hash(data, size);
}

uint64_t helper::xxh3(const void *data, const uint64_t size) {
    return XXH_INLINE_XXH3_64bits(data, size);
}
