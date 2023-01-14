#include "basic_ranges_demo.h"

#include <algorithm>

#include <iostream>
#include <vector>
#include <queue>

struct br_t {
    int n1;
    int n2;
    int n3;
    int n4;
};

std::vector<uint32_t> release_data;

br_t dat_s;

br_t dat[] = {
    {16, 0, 0, 0},
    {15, 0, 0, 1},
    {14, 0, 0, 2},
    {13, 0, 0, 3},
    {12, 0, 0, 4},
    {11, 0, 0, 5},
    {10, 0, 0, 6},
    {9, 0, 0, 7},
    {8, 0, 0, 8},
    {7, 0, 0, 9},
    {6, 0, 0, 10},
    {5, 0, 0, 11},
    {4, 0, 0, 12},
    {3, 0, 0, 13},
    {2, 0, 0, 14},
    {14, 1, 0, 0},
    {13, 1, 0, 1},
    {12, 1, 0, 2},
    {11, 1, 0, 3},
    {10, 1, 0, 4},
    {9, 1, 0, 5},
    {8, 1, 0, 6},
    {7, 1, 0, 7},
    {6, 1, 0, 8},
    {5, 1, 0, 9},
    {4, 1, 0, 10},
    {3, 1, 0, 11},
    {2, 1, 0, 12},
    {14, 0, 1, 0},
    {13, 0, 1, 1},
    {12, 0, 1, 2},
    {11, 0, 1, 3},
    {10, 0, 1, 4},
    {9, 0, 1, 5},
    {8, 0, 1, 6},
    {7, 0, 1, 7},
    {6, 0, 1, 8},
    {5, 0, 1, 9},
    {4, 0, 1, 10},
    {3, 0, 1, 11},
    {2, 0, 1, 12},
    {12, 2, 0, 0},
    {11, 2, 0, 1},
    {10, 2, 0, 2},
    {9, 2, 0, 3},
    {8, 2, 0, 4},
    {7, 2, 0, 5},
    {6, 2, 0, 6},
    {5, 2, 0, 7},
    {4, 2, 0, 8},
    {3, 2, 0, 9},
    {2, 2, 0, 10},
    {12, 1, 1, 0},
    {11, 1, 1, 1},
    {10, 1, 1, 2},
    {9, 1, 1, 3},
    {8, 1, 1, 4},
    {7, 1, 1, 5},
    {6, 1, 1, 6},
    {5, 1, 1, 7},
    {4, 1, 1, 8},
    {3, 1, 1, 9},
    {2, 1, 1, 10},
    {12, 0, 2, 0},
    {11, 0, 2, 1},
    {10, 0, 2, 2},
    {9, 0, 2, 3},
    {8, 0, 2, 4},
    {7, 0, 2, 5},
    {6, 0, 2, 6},
    {5, 0, 2, 7},
    {4, 0, 2, 8},
    {3, 0, 2, 9},
    {2, 0, 2, 10},
    {10, 3, 0, 0},
    {9, 3, 0, 1},
    {8, 3, 0, 2},
    {7, 3, 0, 3},
    {6, 3, 0, 4},
    {5, 3, 0, 5},
    {4, 3, 0, 6},
    {3, 3, 0, 7},
    {2, 3, 0, 8},
    {10, 2, 1, 0},
    {9, 2, 1, 1},
    {8, 2, 1, 2},
    {7, 2, 1, 3},
    {6, 2, 1, 4},
    {5, 2, 1, 5},
    {4, 2, 1, 6},
    {3, 2, 1, 7},
    {2, 2, 1, 8},
    {10, 1, 2, 0},
    {9, 1, 2, 1},
    {8, 1, 2, 2},
    {7, 1, 2, 3},
    {6, 1, 2, 4},
    {5, 1, 2, 5},
    {4, 1, 2, 6},
    {3, 1, 2, 7},
    {2, 1, 2, 8},
    {10, 0, 3, 0},
    {9, 0, 3, 1},
    {8, 0, 3, 2},
    {7, 0, 3, 3},
    {6, 0, 3, 4},
    {5, 0, 3, 5},
    {4, 0, 3, 6},
    {3, 0, 3, 7},
    {2, 0, 3, 8},
    {8, 4, 0, 0},
    {7, 4, 0, 1},
    {6, 4, 0, 2},
    {5, 4, 0, 3},
    {4, 4, 0, 4},
    {3, 4, 0, 5},
    {2, 4, 0, 6},
    {8, 3, 1, 0},
    {7, 3, 1, 1},
    {6, 3, 1, 2},
    {5, 3, 1, 3},
    {4, 3, 1, 4},
    {3, 3, 1, 5},
    {2, 3, 1, 6},
    {8, 2, 2, 0},
    {7, 2, 2, 1},
    {6, 2, 2, 2},
    {5, 2, 2, 3},
    {4, 2, 2, 4},
    {3, 2, 2, 5},
    {2, 2, 2, 6},
    {8, 1, 3, 0},
    {7, 1, 3, 1},
    {6, 1, 3, 2},
    {5, 1, 3, 3},
    {4, 1, 3, 4},
    {3, 1, 3, 5},
    {2, 1, 3, 6},
    {8, 0, 4, 0},
    {7, 0, 4, 1},
    {6, 0, 4, 2},
    {5, 0, 4, 3},
    {4, 0, 4, 4},
    {3, 0, 4, 5},
    {2, 0, 4, 6},
    {6, 5, 0, 0},
    {5, 5, 0, 1},
    {4, 5, 0, 2},
    {3, 5, 0, 3},
    {2, 5, 0, 4},
    {6, 4, 1, 0},
    {5, 4, 1, 1},
    {4, 4, 1, 2},
    {3, 4, 1, 3},
    {2, 4, 1, 4},
    {6, 3, 2, 0},
    {5, 3, 2, 1},
    {4, 3, 2, 2},
    {3, 3, 2, 3},
    {2, 3, 2, 4},
    {6, 2, 3, 0},
    {5, 2, 3, 1},
    {4, 2, 3, 2},
    {3, 2, 3, 3},
    {2, 2, 3, 4},
    {6, 1, 4, 0},
    {5, 1, 4, 1},
    {4, 1, 4, 2},
    {3, 1, 4, 3},
    {2, 1, 4, 4},
    {6, 0, 5, 0},
    {5, 0, 5, 1},
    {4, 0, 5, 2},
    {3, 0, 5, 3},
    {2, 0, 5, 4},
    {4, 6, 0, 0},
    {3, 6, 0, 1},
    {2, 6, 0, 2},
    {4, 5, 1, 0},
    {3, 5, 1, 1},
    {2, 5, 1, 2},
    {4, 4, 2, 0},
    {3, 4, 2, 1},
    {2, 4, 2, 2},
    {4, 3, 3, 0},
    {3, 3, 3, 1},
    {2, 3, 3, 2},
    {4, 2, 4, 0},
    {3, 2, 4, 1},
    {2, 2, 4, 2},
    {4, 1, 5, 0},
    {3, 1, 5, 1},
    {2, 1, 5, 2},
    {4, 0, 6, 0},
    {3, 0, 6, 1},
    {2, 0, 6, 2},
    {2, 7, 0, 0},
    {2, 6, 1, 0},
    {2, 5, 2, 0},
    {2, 4, 3, 0},
    {2, 3, 4, 0},
    {2, 2, 5, 0},
    {2, 1, 6, 0},
    {2, 0, 7, 0},
};

//int dat_size = 4;
int dat_size = 204;
//int dat_size = 1;

//void func(uint32_t prefix, int offset, br_t &r) {
void func(uint32_t prefix, int offset) {

//    std::cout << "build: ";
//    show_br_t(dat[0]);
//    std::cout << " ";
//    show_br_t(dat[1]);
//    printf(" | prefix = %08X | offset = %d\n", prefix, offset);


    if (dat_s.n1 == 0 && dat_s.n2 == 0 && dat_s.n3 == 0 && dat_s.n4 == 0) {
//        printf("release -> %08X\n", prefix);
        release_data.emplace_back(prefix);
        return;
    }

//    for (int i = 0; i < dat_size; ++i) {
//        if (dat[i].n1 == 0 && dat[i].n2 == 0 && dat[i].n3 == 0 && dat[i].n4 == 0) {
////            printf("release -> %08X\n", prefix);
//            return;
//        }
//    }

    if (dat_s.n1 < 0 || dat_s.n2 < 0 || dat_s.n3 < 0 || dat_s.n4 < 0) {
//        std::cout << "break" << std::endl;
        return;
    }

//    bool flag = false;
//    for (int i = 0; i < dat_size; ++i) {
//        if (!(dat[i].n1 < 0 || dat[i].n2 < 0 || dat[i].n3 < 0 || dat[i].n4 < 0)) {
//            flag = true;
//        }
//    }
//    if (!flag) {
//        return;
//    }

    /// generate start with 00
    --dat_s.n1;
    func(prefix | ((uint32_t)0b00 << 30) >> offset, offset + 2);
    ++dat_s.n1;
//    for (int i = 0; i < dat_size; ++i) {
//        --dat[i].n1;
//    }
//    func(prefix | ((uint32_t)0b00 << 30) >> offset, offset + 2);
//    for (int i = 0; i < dat_size; ++i) {
//        ++dat[i].n1;
//    }

    /// generate start with 01
    --dat_s.n2;
    func(prefix | ((uint32_t)0b01 << 30) >> offset, offset + 2);
    ++dat_s.n2;
//    for (int i = 0; i < dat_size; ++i) {
//        --dat[i].n2;
//    }
//    func(prefix | ((uint32_t)0b01 << 30) >> offset, offset + 2);
//    for (int i = 0; i < dat_size; ++i) {
//        ++dat[i].n2;
//    }

    /// generate start with 10
    --dat_s.n3;
    func(prefix | ((uint32_t)0b10 << 30) >> offset, offset + 2);
    ++dat_s.n3;
//    for (int i = 0; i < dat_size; ++i) {
//        --dat[i].n3;
//    }
//    func(prefix | ((uint32_t)0b10 << 30) >> offset, offset + 2);
//    for (int i = 0; i < dat_size; ++i) {
//        ++dat[i].n3;
//    }

    /// generate start with 11
    --dat_s.n4;
    func(prefix | ((uint32_t)0b11 << 30) >> offset, offset + 2);
    ++dat_s.n4;
//    for (int i = 0; i < dat_size; ++i) {
//        --dat[i].n4;
//    }
//    func(prefix | ((uint32_t)0b11 << 30) >> offset, offset + 2);
//    for (int i = 0; i < dat_size; ++i) {
//        ++dat[i].n4;
//    }

}

void func_test(const br_t &start) {

    struct inner_t {
        int n1;
        int n2;
        int n3;
        int n4;
        uint32_t prefix;
        uint32_t offset;
    };

    auto show_inner = [](inner_t &d) {
        printf("(%d, %d, %d, %d) | %08X [%d]\n", d.n1, d.n2, d.n3, d.n4, d.prefix, d.offset);
    };

    std::queue<inner_t> data;

    data.emplace(inner_t {
        .n1 = start.n1,
        .n2 = start.n2,
        .n3 = start.n3,
        .n4 = start.n4,
        .prefix = 0,
        .offset = 0,
    });


    while (!data.empty()) {

//        show_inner(data.front());

        if (data.front().n1 == 0 && data.front().n2 == 0 && data.front().n3 == 0 && data.front().n4 == 0) {
//            printf("%08X\n", data.front().prefix);

            release_data.push_back(data.front().prefix);

            data.pop();
            continue;
        }

        if (data.front().n1 != 0) {
            auto next = inner_t{
                .n1 = data.front().n1 - 1,
                .n2 = data.front().n2,
                .n3 = data.front().n3,
                .n4 = data.front().n4,
                .prefix = data.front().prefix | (((uint32_t) 0b00 << 30) >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        if (data.front().n2 != 0) {
            auto next = inner_t{
                .n1 = data.front().n1,
                .n2 = data.front().n2 - 1,
                .n3 = data.front().n3,
                .n4 = data.front().n4,
                .prefix = data.front().prefix | (((uint32_t) 0b01 << 30) >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        if (data.front().n3 != 0) {
            auto next = inner_t{
                .n1 = data.front().n1,
                .n2 = data.front().n2,
                .n3 = data.front().n3 - 1,
                .n4 = data.front().n4,
                .prefix = data.front().prefix | (((uint32_t) 0b10 << 30) >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }
        if (data.front().n4 != 0) {
            auto next = inner_t{
                .n1 = data.front().n1,
                .n2 = data.front().n2,
                .n3 = data.front().n3,
                .n4 = data.front().n4 - 1,
                .prefix = data.front().prefix | (((uint32_t) 0b11 << 30) >> data.front().offset),
                .offset = data.front().offset + 2,
            };
            data.emplace(next);
        }

        data.pop();
    }


//    std::cout << "Size: " << data.size() << std::endl;

//    data.pop();
//
//    show_inner(data.front());
//
//    data.pop();
//
//    show_inner(data.front());

}


void load_ranges() {
//    std::cout << "ok" << std::endl;

//    for (int n = 0; n <= 7; ++n) { // number of 1x2 and 2x1 block -> 0 ~ 7
//        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) { // number of 2x1 block -> 0 ~ n
//            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
//                int n_1x2 = n - n_2x1;
//                int n_space = 16 - n * 2 - n_1x1;

//                printf("(%d, %d, %d, %d)\n", n_space, n_1x2, n_2x1, n_1x1);
//                printf("{%d, %d, %d, %d},\n", n_space, n_1x2, n_2x1, n_1x1);

                /// 0x0 -> 00 | 1x2 -> 01 | 2x1 -> 10 | 1x1 -> 11
//            }
//        }
//    }


    release_data.reserve(8000000);

//    for (int i = 0; i < dat_size; ++i) {
//        dat_s = dat[i];
//        func(0, 0);
//    }
//    sort(release_data.begin(), release_data.end());

//    func_test(br_t {
//        .n1 = 7,
//        .n2 = 2,
//        .n3 = 1,
//        .n4 = 3,

//        .n1 = 0,
//        .n2 = 1,
//        .n3 = 2,
//        .n4 = 0,
//    });

//    dat_s = {3, 1, 3, 5};
//
//    func(0, 0);

    for (int i = 0; i < dat_size; ++i) {
        func_test(dat[i]);
    }
    // TODO: try merge data
    sort(release_data.begin(), release_data.end());
    std::cout << "size: " << release_data.size() << std::endl;

//    br_t demo = {2, 0, 0, 1};
//    func(0, 0, demo);

//    show_br_t(r);

}
