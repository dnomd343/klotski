#pragma once

#include <vector>
#include <cstdint>

class AllCases {
public:
//    AllCases() {}
//    void find_all_cases();

    void build_basic_ranges();

    const std::vector<uint32_t>* get_basic_ranges();

private:
//    std::vector<uint32_t> all_cases[16];
    std::vector<uint32_t> basic_ranges;

//    void load_basic_ranges();
    void generate_ranges(int n1, int n2, int n3, int n4);
};
