#include "graph.h"
#include "common.h"

#include <list>
#include <iostream>

#include "svg.h"

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


    auto p = Point{100, 200};
    auto rc = RawCode::unsafe_create(0x0603EDF5CAFFF5E2);
    auto gc = GraphCase(p, rc, BLOCK_GAP, BLOCK_LENGTH);

    auto skin = CaseSkin();

    auto svg = SvgGraph(2000, 2000);

    gc.render(svg, skin);

    std::cout << svg.dump() << std::endl;

    return;


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


void GraphCase::render(SvgGraph &svg, const CaseSkin &skin) const {
//    std::cout << "render begin" << std::endl;

    auto raw_code = (uint64_t)code;

    uint32_t case_width = block_width * 4 + block_gap * 5;
    uint32_t case_height = block_width * 5 + block_gap * 6;

    auto skeleton = new SvgRect {start, case_width, case_height};
    skeleton->color = "pink";
    svg.insert(skeleton);

    for (int addr = 0; raw_code; ++addr, raw_code >>= 3) {
        uint32_t block_x = addr % 4;
        uint32_t block_y = (addr - block_x) / 4;

        // TODO: same start point

        switch (raw_code & 0b111) {
            case B_1x1:

                svg.insert(new SvgRect {
                    {
                        start.x + block_x * block_width + (block_x + 1) * block_gap,
                        start.y + block_y * block_width + (block_y + 1) * block_gap,
                    },
                    block_width, block_width,
                });

                break;
            case B_1x2:

                svg.insert(new SvgRect {
                    {
                        start.x + block_x * block_width + (block_x + 1) * block_gap,
                        start.y + block_y * block_width + (block_y + 1) * block_gap,
                    },
                    block_width * 2 + block_gap, block_width,
                });

                break;
            case B_2x1:

                svg.insert(new SvgRect {
                    {
                        start.x + block_x * block_width + (block_x + 1) * block_gap,
                        start.y + block_y * block_width + (block_y + 1) * block_gap,
                    },
                    block_width, block_width * 2 + block_gap,
                });

                break;
            case B_2x2:

                svg.insert(new SvgRect {
                    {
                        start.x + block_x * block_width + (block_x + 1) * block_gap,
                        start.y + block_y * block_width + (block_y + 1) * block_gap,
                    },
                    block_width * 2 + block_gap, block_width * 2 + block_gap,
                });

                break;
            default:
                continue;
        }
    }


}
