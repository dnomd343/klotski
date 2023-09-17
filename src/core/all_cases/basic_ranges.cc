#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include "basic_ranges.h"

static uint32_t range_reverse(uint32_t bin) noexcept {
//    bin = ((bin << 16) & 0xFFFF0000) | ((bin >> 16) & 0x0000FFFF);
//    bin = ((bin << 8) & 0xFF00FF00) | ((bin >> 8) & 0x00FF00FF);
    bin = __builtin_bswap32(bin);
    bin = ((bin << 4) & 0xF0F0F0F0) | ((bin >> 4) & 0x0F0F0F0F);
    return ((bin << 2) & 0xCCCCCCCC) | ((bin >> 2) & 0x33333333);
}

void build(std::vector<uint32_t> &result, int n1, int n2, int n3, int n4) {

    std::vector<int> demo;
    demo.reserve(n1 + n2 + n3 + n4);

    int move_num = 32 - (n1 + n2 + n3 + n4) * 2;

    demo.insert(demo.end(), n1, 0b00);
    demo.insert(demo.end(), n2, 0b01);
    demo.insert(demo.end(), n3, 0b10);
    demo.insert(demo.end(), n4, 0b11);

//    for (int i = 0; i < n1; ++i) {
//        demo.emplace_back(0b00);
//    }
//    for (int i = 0; i < n2; ++i) {
//        demo.emplace_back(0b01);
//    }
//    for (int i = 0; i < n3; ++i) {
//        demo.emplace_back(0b10);
//    }
//    for (int i = 0; i < n4; ++i) {
//        demo.emplace_back(0b11);
//    }

//    for (auto x : demo) {
//        std::cout << x << " ";
//    }
//    std::cout << std::endl;

    do {

        uint32_t tmp = 0;
        for (auto x : demo) {
            tmp <<= 2;
            tmp |= x;
        }
        tmp <<= move_num;
//        volatile auto r = tmp;
        result.emplace_back(tmp);

    } while (next_permutation(demo.begin(), demo.end()));
}

template <class T>
void sort(T begin, T end, T kk) {

    std::vector<uint32_t> tmp;
    tmp.reserve(end - begin);
    auto k1 = begin;
    auto k2 = kk;

    while (1 == 1) {
        if (*k1 < *k2) {
            tmp.emplace_back(*k1);
            ++k1;
            if (k1 == kk) {
                tmp.insert(tmp.end(), k2, end);
                break;
            }
        } else {
            tmp.emplace_back(*k2);
            ++k2;
            if (k2 == end) {
                tmp.insert(tmp.end(), k1, kk);
                break;
            }
        }
    }

    auto p = begin;
    for (auto x : tmp) {
        *p = x;
        ++p;
    }

}

template<class ITER_T>
void sort_v2(ITER_T begin, ITER_T end, ITER_T mid) {

    std::vector<uint32_t> tmp {begin, mid};
    auto k1 = tmp.begin();
    auto k2 = mid;
    auto target = begin;

    while (1 == 1) {

//        std::cout << *k1 << " vs " << *k2 << std::endl;

        if (*k1 < *k2) {
            *target = *k1;
            ++target;
//            tmp.emplace_back(*k1);
            ++k1;
            if (k1 == tmp.end()) {

//                memcpy(&*target, &*k2, end - k2);

//                tmp.insert(tmp.end(), k2, end);
                break;
            }
        } else {
            *target = *k2;
            ++target;
//            tmp.emplace_back(*k2);
            ++k2;
            if (k2 == end) {

//                std::cout << "get it" << std::endl;

//                std::cout << "size = " << (tmp.end() - k1) << std::endl;
//                std::cout << (target - begin)

                memcpy(&*target, &*k1, (tmp.end() - k1) * 4);

//                tmp.insert(tmp.end(), k1, kk);
                break;
            }
        }
    }
//
//    auto p = begin;
//    for (auto x : tmp) {
//        *p = x;
//        ++p;
//    }

}

template<class ITER_T>
void sort_v3(ITER_T begin, ITER_T end, ITER_T mid) {
    std::vector<uint32_t> tmp = {begin, mid};
    auto p = tmp.begin();
    for (;;) {
        if (*p < *mid) {
            *(begin++) = *(p++);
            if (p == tmp.end()) {
                return;
            }
        } else {
            *(begin++) = *(mid++);
            if (mid == end) {
                std::copy(p, tmp.end(), begin);
                return;
            }
        }
    }
}

void demo() {

//    std::vector<uint32_t> demo = {1, 2, 5, 9, 11, 3, 6, 7};
//    auto kk = demo.begin() + 5;
//    for (auto p = demo.begin(); p < kk; ++p) {
//        std::cout << *p << std::endl;
//    }
//    sort_v3(demo.begin(), demo.end(), kk);
//
//    for (auto x : demo) {
//        std::cout << x << std::endl;
//    }
//    return;


//    uint32_t tmp = 0x5129B263; // 0xC98E6845
//
//    for (uint64_t i = 0; i < 0xFFFFFFFF; ++i) {
//        volatile auto r =  range_reverse(tmp);
//    }

//    printf("%08X\n", tmp);

    std::vector<uint32_t> result;

    result.reserve(7311921);

//    build(result, 4, 2, 1, 6);

    std::list<decltype(result.begin())> pp;
    pp.emplace_back(result.begin());

    for (int n = 0; n <= 7; ++n) // number of 1x2 and 2x1 block -> 0 ~ 7
        for (int n_2x1 = 0; n_2x1 <= n; ++n_2x1) // number of 2x1 block -> 0 ~ n
            for (int n_1x1 = 0; n_1x1 <= (14 - n * 2); ++n_1x1) { // number of 1x1 block -> 0 ~ (14 - 2n)
                build(result, 16 - n * 2 - n_1x1, n - n_2x1, n_2x1, n_1x1);
                pp.emplace_back(result.end());
            }

//    std::stable_sort(result.begin(), result.end());
//    return;

//    sort(result.begin() + 363149, result.begin() + 459674, result.begin() + 408194);

    while (1 == 1) {
        auto begin = pp.begin();
        while (1 == 1) {
            auto mid = begin;
            ++mid;
            if (mid == pp.end()) {
                break;
            }
            auto end = mid;
            ++end;
            if (end == pp.end()) {
                break;
            }
//            std::cout << (*begin - result.begin()) << " " << (*mid - result.begin()) << " " << (*end - result.begin()) << std::endl;
            sort_v3(*begin, *end, *mid);
            ++begin;
            ++begin;
            pp.erase(mid);
        }
        if (pp.size() == 2) {
            break;
        }
    }


//    std::sort(result.begin(), result.end());
//    std::stable_sort(result.begin(), result.end());

    for (auto &x : result) {
        x = range_reverse(x);
    }

    for (auto x : result) {
        printf("%08X\n", x);
    }

//    for (uint32_t i = 0; i < 0xFF; ++i) {
//        build(result, 4, 2, 1, 6);
//    }

}
