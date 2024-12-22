#include "exception.h"
#include "py_ffi/group.h"
#include "py_ffi/layout.h"

using klotski::codec::ShortCode;
using klotski::codec::CommonCode;

using klotski::ffi::PyExc_GroupError;

using klotski::ffi::PyGroupUnion;

static klotski::group::GroupUnion convert(uint8_t type_id) {
    auto group_union = klotski::group::GroupUnion::create(type_id);
    if (!group_union.has_value()) {
        throw PyExc_GroupError(std::format("invalid type id: {}", type_id));
    }
    return group_union.value();
}

PyGroupUnion::PyGroupUnion(const uint8_t type_id) :
    group_union_(convert(type_id)) {}

PyGroupUnion::PyGroupUnion(const PyShortCode short_code) :
    group_union_(GroupUnion::from_short_code(std::bit_cast<ShortCode>(short_code))) {}

PyGroupUnion::PyGroupUnion(const PyLayout layout) :
    group_union_(GroupUnion::from_common_code(std::bit_cast<CommonCode>(layout))) {}

uint32_t PyGroupUnion::value() const {
    return group_union_.unwrap();
}

uint32_t PyGroupUnion::size() const {
    return group_union_.size();
}

uint32_t PyGroupUnion::group_num() const {
    return group_union_.group_num();
}

uint32_t PyGroupUnion::pattern_num() const {
    return group_union_.pattern_num();
}

uint32_t PyGroupUnion::max_group_size() const {
    return group_union_.max_group_size();
}

klotski::ffi::PyCases PyGroupUnion::cases() const {
    auto cases = PyCases::from(group_union_.cases());
    return cases;
}

std::vector<klotski::ffi::PyGroup> PyGroupUnion::groups() const {
    std::vector<PyGroup> groups;
    groups.reserve(group_num());
    for (auto group : group_union_.groups()) {
        groups.emplace_back(std::bit_cast<PyGroup>(group));
    }
    return groups;
}
