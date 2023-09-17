#include <iostream>
#include <algorithm>

#include "all_cases/basic_ranges.h"

int main() {

    auto start = clock();

    demo();


//    for (int n = 0; n <= 7; ++n) // number of 1x2 and 2x1 block -> 0 ~ 7
//        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) // number of 2x1 block -> 0 ~ n
//            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) // number of 1x1 block -> 0 ~ (14 - 2n)
//                build(16 - n * 2 - n_1x1, n - n_2x1, n_2x1, n_1x1);



//                generate(generate_t { // generate target ranges
//                        .n1 = 16 - n * 2 - n_1x1, /// space -> 00
//                        .n2 = n - n_2x1, /// 1x2 -> 01
//                        .n3 = n_2x1, /// 2x1 -> 10
//                        .n4 = n_1x1, /// 1x1 -> 11
//                });


//    std::stable_sort(result.begin(), result.end());

//    build(4, 2, 1, 6);

    std::cerr << ((clock() - start) * 1000 / CLOCKS_PER_SEC) << "ms" << std::endl;

//    std::cout << result.size() << std::endl;

//    std::vector<uintmax_t> demo {0b00, 0b01, 0b10, 0b11};

//    do {
//        std::cout << demo[0] << " " << demo[1] << " " << demo[2] << " " << demo[3] << std::endl;
//    } while (next_permutation(demo.begin(), demo.end()));

    return 0;
}
