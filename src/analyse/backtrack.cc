#include "analyse.h"

#include <iostream>

#include <set>
#include <unordered_set>

// TODO: using const RawCode& instead of uint64_t

namespace std {
    template<>
    struct hash<Analyse::backtrack_t> {
        std::size_t operator()(const Analyse::backtrack_t &b) const {
//            std::cout << "get hash: " << b.code << std::endl;
            return std::hash<uint64_t>()(b.code);
        }
    };

    template<>
    struct equal_to<Analyse::backtrack_t> {
        bool operator()(const Analyse::backtrack_t &b1, const Analyse::backtrack_t &b2) const {
//            std::cout << "get eq: " << b1.code << " ? " << b2.code << std::endl;
            return b1.code == b2.code;
        }
    };
}

void Analyse::backtrack_demo(uint64_t code) {

    // TODO: confirm code exist
    std::cout << cases[code].step << std::endl;

//    std::vector<std::unordered_set<backtrack_t>> dat;

    std::unordered_set<backtrack_t> test;


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
