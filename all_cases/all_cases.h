#pragma once

#include <iostream>
#include <vector>

class AllCases {
public:
    AllCases() {
        load_basic_ranges();
    }
    void find_all_cases();
    std::vector<uint32_t> all_cases[16];
    std::vector<uint32_t> basic_ranges;

private:
    void load_basic_ranges();
    void generate_ranges(int n1, int n2, int n3, int n4);
    static bool check_case(uint32_t head, uint32_t range);
};
