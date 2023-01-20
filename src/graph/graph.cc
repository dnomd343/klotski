#include "graph.h"
#include "common.h"

#include <list>
#include <iostream>

#include "svg.h"

void Graph::svg_demo(Analyse::track_data_t track_data) {


    auto s = SvgRect();

    s.left = 50;
    s.top = 80;

    s.width = 100;
    s.height = 200;

    s.radius = 20;

    s.opacity = 0.8;
    s.line_opacity = 0.5;

    s.color = "blue";
    s.line_color = "green";

//    printf(R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="%d" height="%d">)", 1000, 1000);
//    printf("\n");
//
//    std::cout << "  " << s.dump() << std::endl;
//
//    printf("</svg>\n");

    auto l = SvgLine();
    l.start_x = 100;
    l.start_y = 200;

    l.end_x = 300;
    l.end_y = 400;

    auto sg = SvgGraph();
//    sg.insert(&s);
//    sg.insert(&l);

    sg.insert(std::move(s));
    sg.insert(std::move(l));

    sg.dump();

    return;



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

//    SvgCase test = SvgCase();
//    test.width = BLOCK_LENGTH;
//    test.gap = BLOCK_GAP;
//    test.left = 50;
//    test.top = 100;
//    test.code = layer_data[0][0].code;
//    test.render();

    for (uint32_t i = 0; i < layer_data.size(); ++i) {

        uint64_t CASE_Y = CASE_GAP_Y * (i + 1) + CASE_HEIGHT * i;

//        std::cout << "TOP: " << CASE_Y << std::endl;

        uint64_t left_offset = (MAIN_WIDTH - (CASE_GAP_X * (layer_data[i].size() + 1) + CASE_WIDTH * layer_data[i].size())) / 2;

        for (uint32_t j = 0; j < layer_data[i].size(); ++j) {
            uint64_t CASE_X = CASE_GAP_X * (j + 1) + CASE_WIDTH * j + left_offset;
//            printf("(%ld, %ld, %ld, %ld)\n", CASE_Y, CASE_X, CASE_WIDTH, CASE_HEIGHT);

//            printf(R"(  <rect x="%ld" y="%ld" width="%ld" height="%ld" style="fill:pink;stroke-width:1;stroke:blue"/>)", CASE_X, CASE_Y, CASE_WIDTH, CASE_HEIGHT);
//            printf("\n");

//            SvgCase tmp = SvgCase();
//            tmp.width = BLOCK_LENGTH;
//            tmp.gap = BLOCK_GAP;
//            tmp.left = CASE_X;
//            tmp.top = CASE_Y;
//            tmp.code = layer_data[i][j].code;
//            tmp.render();

        }

    }

    printf("</svg>\n");


}

//void SvgRect::to_xml() const {
//    // TODO: return std::string
//    printf(R"(<rect x="%ld" y="%ld" width="%ld" height="%ld" style="fill:pink;stroke-width:1;stroke:blue"/>)", left, top, width, height);
//    printf("\n");
//}

//void SvgCase::render() const {
//
//    std::string result;
//
//    uint64_t raw_code = code;
//
////    std::cout << "start rander" << std::endl;
//
//    auto tmp = SvgRect();
//
//    tmp.left = left;
//    tmp.top = top;
//    tmp.width = width * 4 + gap * 5;
//    tmp.height = width * 5 + gap * 6;
////    std::cout << "main" << std::endl;
//    tmp.to_xml();
//
//    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) {
//
//        uint32_t block_x = addr % 4;
//        uint32_t block_y = (addr - block_x) / 4;
//
//        switch (raw_code & 0b111) {
//            case B_1x1:
//
//                tmp.top = width * block_y + gap * (block_y + 1) + top;
//                tmp.left = width * block_x + gap * (block_x + 1) + left;
//                tmp.height = width;
//                tmp.width = width;
////                std::cout << "1x1" << std::endl;
//                tmp.to_xml();
//
//                break;
//            case B_1x2:
//
//                tmp.top = width * block_y + gap * (block_y + 1) + top;
//                tmp.left = width * block_x + gap * (block_x + 1) + left;
//                tmp.height = width;
//                tmp.width = width * 2 + gap;
////                std::cout << "1x2" << std::endl;
//                tmp.to_xml();
//
//                break;
//            case B_2x1:
//
//                tmp.top = width * block_y + gap * (block_y + 1) + top;
//                tmp.left = width * block_x + gap * (block_x + 1) + left;
//                tmp.height = width * 2 + gap;
//                tmp.width = width;
////                std::cout << "2x1" << std::endl;
//                tmp.to_xml();
//
//                break;
//            case B_2x2:
//
//                tmp.top = width * block_y + gap * (block_y + 1) + top;
//                tmp.left = width * block_x + gap * (block_x + 1) + left;
//                tmp.height = width * 2 + gap;
//                tmp.width = width * 2 + gap;
////                std::cout << "2x2" << std::endl;
//                tmp.to_xml();
//
//                break;
//            default:
//                continue;
//        }
//    }
//
//}
