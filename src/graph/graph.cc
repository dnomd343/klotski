#include "graph.h"
#include "common.h"

#include <list>
#include <iostream>

#include "svg.h"

std::string Graph::svg_demo(Analyse::track_data_t track_data) {

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


    auto skin = CaseSkin();

    // TODO: remove layer_index
    // TODO: try remove layer_num
    struct graph_t {
        uint64_t code;
//        uint32_t layer_num;
//        uint32_t layer_index;
        std::list<uint32_t> next;
    };

    std::unordered_map<uint64_t, graph_t> graph_data;
    std::vector<std::vector<graph_t*>> graph_layer(track_data.size());

    for (uint32_t i = 0; i < track_data.size(); ++i) {
        for (const auto &c : track_data[i]) {
            if (c.second.next.empty()) { // without next cases

//                std::cout << "layer = " << i << " | code = " << c.second.code << std::endl;

                graph_layer[i].emplace_back(&graph_data.emplace(c.second.code, graph_t {
                    .code = c.second.code,
//                    .layer_num = i,
//                    .layer_index = static_cast<uint32_t>(graph_layer[i].size()),
                    .next = std::list<uint32_t>{},
                }).first->second);

            }
        }
    }

    for (uint32_t i = graph_layer.size() - 1; i > 0; --i) {

//        std::cout << std::endl;
//        std::cout << "layer " << i << " -> " << graph_layer[i].size() << std::endl;

//        for (const auto *curr : graph_layer[i]) {
        for (uint32_t layer_index = 0; layer_index < graph_layer[i].size(); ++layer_index) {
//            std::cout << "code = " << c->code << std::endl;

            const auto *curr = graph_layer[i][layer_index];

            for (const auto *src : track_data[i][curr->code].last) {
//                std::cout << "  src -> " << src->code << std::endl;

                auto find_ret = graph_data.find(src->code);

                if (find_ret != graph_data.end()) { // already exist

//                    find_ret->second.next.emplace_back(curr->layer_index);
                    find_ret->second.next.emplace_back(layer_index);

                } else {
                    graph_layer[i - 1].emplace_back(&graph_data.emplace(src->code, graph_t {
                        .code = src->code,
//                        .layer_num = i - 1,
//                        .layer_index = static_cast<uint32_t>(graph_layer[i - 1].size()),
//                            .next = std::list<uint32_t>{curr->layer_index},
                        .next = std::list<uint32_t>{layer_index},
                    }).first->second);

                }

            }
        }
    }

    uint32_t max_width = 0;
    uint32_t max_height = graph_layer.size();
    for (const auto &layer : graph_layer) {
        max_width = (layer.size() > max_width) ? layer.size() : max_width;
    }
//    std::cout << "max width -> " << max_width << std::endl;


    uint64_t graph_width = CASE_WIDTH * max_width + CASE_GAP_X * (max_width + 1);
    uint64_t graph_height = CASE_HEIGHT * max_height + CASE_GAP_Y * (max_height + 1);


    auto case_address = [this, &graph_width, &graph_layer](uint32_t i, uint32_t j) -> Point {
        uint64_t case_top = CASE_GAP_Y * (i + 1) + CASE_HEIGHT * i;
        uint64_t offset = (graph_width - (CASE_GAP_X * (graph_layer[i].size() + 1) + CASE_WIDTH * graph_layer[i].size())) / 2;
        uint64_t case_left = offset + CASE_GAP_X * (j + 1) + CASE_WIDTH * j;

        return Point {case_left, case_top};
    };

    auto svg = SvgGraph(graph_width, graph_height);

//    std::cout << graph_layer[0][0]->next.size() << std::endl;

    for (uint32_t i = 0; i < graph_layer.size(); ++i) {
//    for (uint32_t i = 0; i < 1; ++i) {

//        uint64_t case_top = CASE_GAP_Y * (i + 1) + CASE_HEIGHT * i;

//        uint64_t offset = (graph_width - (CASE_GAP_X * (graph_layer[i].size() + 1) + CASE_WIDTH * graph_layer[i].size())) / 2;

        for (uint32_t j = 0; j < graph_layer[i].size(); ++j) {
//            uint64_t case_left = offset + CASE_GAP_X * (j + 1) + CASE_WIDTH * j;

//            auto g = GraphCase({case_left, case_top}, graph_layer[i][j]->code, BLOCK_GAP, BLOCK_LENGTH);
            auto g = GraphCase(case_address(i, j), graph_layer[i][j]->code, BLOCK_GAP, BLOCK_LENGTH);
            g.render(svg, skin);

            for (const auto next : graph_layer[i][j]->next) {

                auto s = case_address(i, j);
                auto e = case_address(i + 1, next);

                s.x += CASE_WIDTH / 2;
                s.y += CASE_HEIGHT + LINE_OFFSET;

                e.x += CASE_WIDTH / 2;
                e.y -= LINE_OFFSET;

                auto line = new SvgLine(s, e);
                svg.insert(line);
            }

        }
    }

    return svg.dump();

}
