#include <stdio.h>
#include "klotski.h"

int main() {
//    printf("cli boot\n");

//    tmain();

    bool ret = common_code_check(0x1A9BF0C00);
    printf("result -> %d\n", ret);

//    printf("cli exit\n");
    return 0;
}
