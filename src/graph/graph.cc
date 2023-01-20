#include "graph.h"

#include <iostream>

void Graph::svg_demo(Analyse::track_data_t track_data) {


    for (uint32_t i = 0; i < track_data.size(); ++i) {

        const auto &ly = track_data[i];

        std::cout << std::endl;
        std::cout << "----------------------------------";
        std::cout << " layer " << i << " ";
        std::cout << "----------------------------------" << std::endl;

        for (const auto &c : ly) {
            for (const auto &l : c.second.last) {
                std::cout << l->code << " ";
            }
            std::cout << " <- [" << c.second.code << "] -> ";
            for (const auto &n : c.second.next) {
                std::cout << n->code << " ";
            }
            std::cout << std::endl;
        }

    }

}
