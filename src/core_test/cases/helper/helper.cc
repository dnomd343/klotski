#include <gtest/gtest.h>

#include "cases.h"

using klotski::cases::TYPE_ID_LIMIT;

void type_id_parallel(std::function<void(uint32_t type_id)> &&func) {

    BS::thread_pool pool;

    // TODO: using `detach_sequence`
    for (uint32_t type_id = 0; type_id < TYPE_ID_LIMIT; ++type_id) {

        pool.detach_task([type_id, func = std::move(func)]() {
            func(type_id);
        });

    }

    pool.wait();

}
