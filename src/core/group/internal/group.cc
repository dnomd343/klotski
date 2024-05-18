#include "group/group.h"

/// 1. n_1x1 + (n_1x2 + n_2x1) * 2 <= 14
/// 2. (n_1x1 != 0) && (n_2x1 != 7)
struct block_num_t {
    uint8_t n_1x1 = 0; /// [0, 14]
    uint8_t n_1x2 = 0; /// [0, 7]
    uint8_t n_2x1 = 0; /// [0, 7]
};
/// n_space = 16 - n_1x1 - (n_1x2 + n_2x1) * 2

// TODO: fast convert from RawCode / CommonCode -> block_num_t
static block_num_t block_num(klotski::codec::RawCode raw_code);
static block_num_t block_num(klotski::codec::CommonCode common_code);

// TODO: should we impl type_id -> block_num ?

// TODO: convert from block_num -> type_id
static int type_id(block_num_t block_num);
