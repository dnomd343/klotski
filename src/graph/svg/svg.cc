#include "svg.h"

#include <iostream>


SvgGraph::~SvgGraph() {
    for (auto *object : objects) {
        delete object;
    }
}

void SvgGraph::insert(SvgObject *obj) {
    objects.emplace_back(obj);
}

std::string SvgLine::dump() const {
    /// basic attributes of svg-line

    return "<line />";

}

std::string SvgRect::dump() const {
    /// basic attributes of svg-rect
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

std::string SvgGraph::dump() const {
    std::string xml = R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1")";
    xml += " width=\"" + std::to_string(width) + "\"";
    xml += " height=\"" + std::to_string(height) + "\">\n";
    for (const auto *object : objects) {
        xml += "  " + object->dump() + "\n";
    }
    return xml + "</svg>\n";
}
