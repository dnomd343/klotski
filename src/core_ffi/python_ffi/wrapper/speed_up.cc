#include "py_ffi/speed_up.h"

#include <group/group.h>
#include <all_cases/all_cases.h>

using klotski::ffi::PySpeedUp;

using klotski::cases::AllCases;
using klotski::cases::BasicRanges;

using klotski::group::GroupCases;

void PySpeedUp::stage_0() {
    BasicRanges::instance().build();
}

void PySpeedUp::stage_1() {
    AllCases::instance().build();
}

void PySpeedUp::stage_2() {
    GroupCases::build();
}

bool PySpeedUp::is_stage_0() {
    return BasicRanges::instance().is_available();
}

bool PySpeedUp::is_stage_1() {
    return AllCases::instance().is_available();
}

bool PySpeedUp::is_stage_2() {
    return GroupCases::is_fast_mode();
}
