#include "analyse.h"

#include <iostream>

#include <set>
#include <unordered_set>

// TODO: using const RawCode& instead of uint64_t

struct backtrack_hash {
//    template <class T1, class T2>
//    std::size_t operator()(const std::pair<T1, T2> &v) const {
//        return std::hash<T1>()(v.size());
//    }

    std::size_t operator()(const Analyse::backtrack_t &b) const {
        std::cout << "hash -> " << b.code << std::endl;

        auto hash_ret = std::hash<uint64_t>()(b.code);

        std::cout << "hash ret -> " << hash_ret << std::endl;

        return b.code;
    }

};

void Analyse::backtrack_demo(uint64_t code) {

//    std::cout << RawCode(code) << std::endl;

    // TODO: confirm code exist
    std::cout << cases[code].step << std::endl;

//    std::vector<std::unordered_set<backtrack_t>> dat;

    auto b_hash = [](const backtrack_t &b) {
        auto hash_dat = std::hash<uint64_t>()(b.code);
        std::cout << "get hash: " << b.code << " -> " << hash_dat << std::endl;
        return hash_dat;
    };

    auto b_eq = [](const backtrack_t &b1, const backtrack_t &b2) {
        std::cout << "get eq: " << b1.code << " ? " << b2.code << std::endl;
        return b1.code == b2.code;
    };

    std::unordered_set<backtrack_t, decltype(b_hash), decltype(b_eq)> test(0, b_hash, b_eq);

//    test.emplace(backtrack_t {
//        .code = 123,
//        .layer_num = 0,
//        .last = std::list<backtrack_t*>{},
//        .next = std::list<backtrack_t*>{},
//    });
//
//    test.emplace(backtrack_t {
//        .code = 233,
//        .layer_num = 1,
//        .last = std::list<backtrack_t*>{},
//        .next = std::list<backtrack_t*>{},
//    });
//
//    test.emplace(backtrack_t {
//        .code = 343,
//        .layer_num = 2,
//        .last = std::list<backtrack_t*>{},
//        .next = std::list<backtrack_t*>{},
//    });

    for (uint32_t i = 0; i < 20; ++i) {
        test.emplace(backtrack_t {
            .code = i * 1000,
            .layer_num = 1,
            .last = std::list<backtrack_t*>{},
            .next = std::list<backtrack_t*>{},
        });
    }

    std::cout << "------------------------" << std::endl;
    std::cout << "hashmap size: " << test.size() << std::endl;
    std::cout << "bucket count: " << test.bucket_count() << std::endl;
    std::cout << "load factor: " << test.load_factor() << std::endl;

    for (uint32_t i = 0; i < test.bucket_count(); ++i) {
        std::cout << "bucket " << i << " -> " << test.bucket_size(i) << std::endl;
    }

}
