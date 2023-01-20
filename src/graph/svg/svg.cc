#include "svg.h"

std::string SvgRect::dump() const {
    /// basic attribute of svg-rect
    std::string xml = "<rect ";
    xml += "x=\"" + std::to_string(left) + "\" ";
    xml += "y=\"" + std::to_string(top) + "\" ";
    xml += "width=\"" + std::to_string(width) + "\" ";
    xml += "height=\"" + std::to_string(height) + "\" ";
    if (radius != 0) {
        xml += "rx=\"" + std::to_string(radius) + "\" ";
    }
    /// style attribute of svg-rect
    std::string style = "stroke-width:" + std::to_string(stroke) + ";";
    if (!color.empty()) {
        style += "fill:" + color + ";";
    }
    if (!line_color.empty()) {
        style += "stroke:" + line_color + ";";
    }
    if (opacity != 0) {
        style += "fill-opacity:" + std::to_string(opacity).substr(0, 4) + ";";
    }
    if (line_opacity != 0) {
        style += "stroke-opacity:" + std::to_string(line_opacity).substr(0, 4) + ";";
    }
    return xml + "style=\"" + style + "\" />";
}


SvgGraph::~SvgGraph() {
    for (auto &object : objects) {
        delete object;
    }
}

//void SvgGraph::insert(SvgObject *obj) {
//    objects.emplace_back(obj);
//}

#include <iostream>

std::string SvgGraph::dump() const {

    for (auto o : objects) {
        std::cout << o->dump() << std::endl;
    }

    return "svg result";
}

void SvgGraph::insert(const SvgRect &rect) {
//void SvgGraph::insert(SvgRect rect) {

    auto s = new SvgRect(rect);
    objects.emplace_back(s);
}

void SvgGraph::insert(const SvgLine &line) {

    auto l = new SvgLine(line);
    objects.emplace_back(l);

}

std::string SvgLine::dump() const {

    return "<line />";

}
