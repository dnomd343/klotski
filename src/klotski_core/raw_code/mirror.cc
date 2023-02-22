#include "raw_code.h"

using klotski::RawCode;

/// Static mirror functions
bool RawCode::is_vertical_mirror(RawCode &&raw_code) {
    return raw_code.is_vertical_mirror();
}

bool RawCode::is_horizontal_mirror(RawCode &&raw_code) {
    return raw_code.is_horizontal_mirror();
}

bool RawCode::is_vertical_mirror(const RawCode &raw_code) {
    return raw_code.is_vertical_mirror();
}

bool RawCode::is_horizontal_mirror(const RawCode &raw_code) {
    return raw_code.is_horizontal_mirror();
}

RawCode RawCode::to_vertical_mirror(RawCode &&raw_code) {
    return raw_code.to_vertical_mirror();
}

RawCode RawCode::to_horizontal_mirror(RawCode &&raw_code) {
    return raw_code.to_horizontal_mirror();
}

RawCode RawCode::to_vertical_mirror(const RawCode &raw_code) {
    return raw_code.to_vertical_mirror();
}

RawCode RawCode::to_horizontal_mirror(const RawCode &raw_code) {
    return raw_code.to_horizontal_mirror();
}

/// Mirror check functions
bool RawCode::is_vertical_mirror() const {

    // TODO: vertical mirror check

}

bool RawCode::is_horizontal_mirror() const {

    // TODO: horizontal mirror check

}

/// Mirror convert functions
RawCode RawCode::to_vertical_mirror() const {

    // TODO: vertical mirror convert

}

RawCode RawCode::to_horizontal_mirror() const {

    // TODO: horizontal mirror convert

}
