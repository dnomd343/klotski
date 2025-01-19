#pragma once

namespace klotski::codec {

constexpr uint64_t RawCode::get_vertical_mirror(const uint64_t raw_code) {
    uint64_t code = ((raw_code >> 36) & 0x000'000'000'fff'fff) | ((raw_code << 36) & 0xfff'fff'000'000'000);
    code = ((code >> 12) & 0x000'fff'000'000'fff) | ((code << 12) & 0xfff'000'000'fff'000);
    code |= (raw_code & 0x000'000'fff'000'000);
    uint64_t m1 = ~code & (code >> 1) & 0x249'249'249'249'249;
    uint64_t m2 = ~code & (code >> 1) & 0x492'492'492'492'492;
    m1 |= (m1 << 2);
    m2 |= (m2 >> 1);
    return (code | m1 | m2) & ~(m1 >> 12) & ~(m2 >> 12);
}

constexpr uint64_t RawCode::get_horizontal_mirror(const uint64_t raw_code) {
    uint64_t code = ((raw_code >> 6) & 0x03f'03f'03f'03f'03f) | ((raw_code << 6) & 0xfc0'fc0'fc0'fc0'fc0);
    code = ((code >> 3) & 0x1c7'1c7'1c7'1c7'1c7) | ((code << 3) & 0xe38'e38'e38'e38'e38);
    uint64_t m1 = ~code & (code << 1) & 0x492'492'492'492'492;
    uint64_t m2 = ~code & (code >> 1) & 0x492'492'492'492'492;
    m1 |= (m1 << 1);
    m2 |= (m2 >> 1);
    return (code | m1 | m2) & ~(m1 >> 3) & ~(m2 >> 3);
}

constexpr uint64_t RawCode::get_diagonal_mirror(const uint64_t raw_code) {
    // TODO: perf it
    return get_horizontal_mirror(get_vertical_mirror(raw_code));
}

constexpr bool RawCode::check_mirror(const uint64_t raw_code) {
    const uint64_t m1 = ~raw_code & (raw_code << 1) & 0x492'492'492'492'492;
    const uint64_t m2 = raw_code & 0x924'924'924'924'924;
    const uint64_t code = ~((m1 | (m1 << 1)) << 3) & (raw_code | (m2 >> 1) | (m2 >> 2));
    return !(0x007'007'007'007'007 & ((code >> 9) ^ code))
        && !(0x038'038'038'038'038 & ((code >> 3) ^ code));
}

} // namespace klotski::codec
