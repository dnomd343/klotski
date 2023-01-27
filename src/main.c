#include <stdio.h>
#include "klotski.h"

int main() {
//    printf("cli boot\n");

//    tmain();

//    bool ret = common_code_check(0x1A9BF0C00);
//    printf("result -> %d\n", ret);

    uint64_t raw_code;
    uint32_t short_code;
    uint64_t common_code = 0x1A9BF0C00;

    short_code_enable_fast();

    if (!common_code_to_raw_code(common_code, &raw_code)) {
        printf("error\n");
    } else {
        printf("raw code -> %015lX\n", raw_code);
    }

    if (!common_code_to_short_code(common_code, &short_code)) {
        printf("error\n");
    } else {
        printf("short code -> %d\n", short_code);
    }

//    printf("cli exit\n");
    return 0;
}
