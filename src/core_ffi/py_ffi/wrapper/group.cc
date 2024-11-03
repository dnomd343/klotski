#include "py_ffi/group.h"

using namespace klotski::ffi;

uint32_t PyGroup::type_id() const {
    return group_.type_id();
}

uint32_t PyGroup::pattern_id() const {
    return group_.pattern_id();
}

std::string PyGroup::to_string() const {
    return group_.to_string();
}

PyCases PyGroup::cases() const {
    return PyCases::from(group_.cases());
}

uint32_t PyGroup::size() const {
    return group_.size();
}

bool PyGroup::is_vertical_mirror() const {
    return group_.is_vertical_mirror();
}

bool PyGroup::is_horizontal_mirror() const {
    return group_.is_horizontal_mirror();
}

PyGroup PyGroup::to_vertical_mirror() const {
    return std::bit_cast<PyGroup>(group_.to_vertical_mirror());
}

PyGroup PyGroup::to_horizontal_mirror() const {
    return std::bit_cast<PyGroup>(group_.to_horizontal_mirror());
}
