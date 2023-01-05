#pragma once

#include <cstdint>

class ShortCode {
public:
    uint32_t zip_short_code(uint64_t code);
    uint64_t unzip_short_code(uint32_t short_code);

};
