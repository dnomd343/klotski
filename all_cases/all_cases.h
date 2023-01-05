#pragma once

#include <iostream>
#include <vector>

class AllCases {
public:
//    std::vector<uint64_t> all_cases;

    std::vector<uint32_t> all_cases[16]; // 012 456 89A CDE

    std::vector<uint32_t> basic_ranges;
    void find_all_cases();

//protected:
    static bool check_case(uint32_t head, uint32_t range);
    void load_basic_ranges();

private:
    void build_basic_ranges(int n1, int n2, int n3, int n4);

};
