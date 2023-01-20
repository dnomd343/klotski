#include "graph.h"

#include <list>
#include <iostream>

void Graph::svg_demo(Analyse::track_data_t track_data) {

//
//    for (uint32_t i = 0; i < track_data.size(); ++i) {
//
//        const auto &ly = track_data[i];
//
//        std::cout << std::endl;
//        std::cout << "----------------------------------";
//        std::cout << " layer " << i << " ";
//        std::cout << "----------------------------------" << std::endl;
//
//        for (const auto &c : ly) {
//            for (const auto &l : c.second.last) {
//                std::cout << l->code << " ";
//            }
//            std::cout << " <- [" << c.second.code << "] -> ";
//            for (const auto &n : c.second.next) {
//                std::cout << n->code << " ";
//            }
//            std::cout << std::endl;
//        }
//
//    }

    struct inner_t {
        uint64_t code;
        uint32_t layer_num;
        uint32_t layer_index;
        std::list<uint32_t> next;
    };

    std::vector<std::vector<inner_t>> layer_data(track_data.size());

    for (uint32_t i = 0; i < track_data.size(); ++i) {

        for (const auto &c : track_data[i]) {
            layer_data[i].emplace_back(inner_t {
                .code = c.second.code,
                .layer_num = c.second.layer_num,
                .layer_index = (uint32_t)layer_data[i].size(),
                .next = std::list<uint32_t>{},
            });
        }

    }


    uint32_t max_length_num = 0;
    for (const auto &ld : layer_data) {
        if (ld.size() > max_length_num) {
            max_length_num = ld.size();
        }
    }
//    std::cout << "max length -> " << max_length_num << std::endl;


    uint64_t MAIN_WIDTH = CASE_WIDTH * max_length_num + CASE_GAP_X * (max_length_num + 1);
    uint64_t MAIN_HEIGHT = CASE_HEIGHT * layer_data.size() + CASE_GAP_Y * (layer_data.size() + 1);

//    std::cout << "MAIN_WIDTH = " << MAIN_WIDTH << std::endl;
//    std::cout << "MAIN_HEIGHT = " << MAIN_HEIGHT << std::endl;

    printf(R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="%ld" height="%ld">)", MAIN_WIDTH, MAIN_HEIGHT);
    printf("\n");

    for (uint32_t i = 0; i < layer_data.size(); ++i) {

        uint64_t CASE_Y = CASE_GAP_Y * (i + 1) + CASE_HEIGHT * i;

//        std::cout << "TOP: " << CASE_Y << std::endl;

        uint64_t left_offset = (MAIN_WIDTH - (CASE_GAP_X * (layer_data[i].size() + 1) + CASE_WIDTH * layer_data[i].size())) / 2;

        for (uint32_t j = 0; j < layer_data[i].size(); ++j) {
            uint64_t CASE_X = CASE_GAP_X * (j + 1) + CASE_WIDTH * j + left_offset;
//            printf("(%ld, %ld, %ld, %ld)\n", CASE_Y, CASE_X, CASE_WIDTH, CASE_HEIGHT);

            printf(R"(  <rect x="%ld" y="%ld" width="%ld" height="%ld" style="fill:pink;stroke-width:1;stroke:blue"/>)", CASE_X, CASE_Y, CASE_WIDTH, CASE_HEIGHT);
            printf("\n");

        }

    }

    printf("</svg>\n");


}
