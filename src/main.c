#include <stdio.h>
#include "klotski.h"

int main() {
//    printf("cli boot\n");

//    tmain();
    return 0;

//    bool ret = common_code_check(0x1A9BF0C00);
//    printf("result -> %d\n", ret);

    uint64_t raw_code;
    uint32_t short_code;
//    uint64_t common_code = 0x1A9BF0C00;
    uint64_t common_code = 0x4FEA13400;

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

    char common_code_str[COMMON_CODE_STR_SIZE];
    if (!common_code_to_string(common_code, common_code_str)) {
        printf("error\n");
    } else {
        printf("string -> %s\n", common_code_str);
    }
    if (!common_code_to_string_shorten(common_code, common_code_str)) {
        printf("error\n");
    } else {
        printf("string shorten -> %s\n", common_code_str);
    }
    if (!common_code_from_string(common_code_str, &common_code)) {
        printf("error\n");
    } else {
        printf("common code -> %09lX\n", common_code);
    }

    char short_code_str[SHORT_CODE_STR_SIZE];
    if (!short_code_to_string(short_code, short_code_str)) {
        printf("error\n");
    } else {
        printf("string -> %s\n", short_code_str);
    }
    if (!short_code_from_string(short_code_str, &short_code)) {
        printf("error\n");
    } else {
        printf("short code -> %d\n", short_code);
    }

    printf("version -> %d.%d.%d\n", get_version_major(), get_version_minor(), get_version_patch());

//    printf("cli exit\n");
    return 0;
}
