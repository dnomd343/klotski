#include "svg.h"

void SvgGraph::insert(SvgObject *obj) {
    objects.emplace_back(obj);
}

SvgGraph::~SvgGraph() {
    for (auto *object : objects) {
        delete object;
    }
}

std::string SvgLine::dump() const {

    /// basic attributes of svg-line

    std::string xml = "<line ";
    xml += "x1=\"" + std::to_string(start.x) + "\" ";
    xml += "y1=\"" + std::to_string(start.y) + "\" ";
    xml += "x2=\"" + std::to_string(end.x) + "\" ";
    xml += "y2=\"" + std::to_string(end.y) + "\" ";

    std::string style = "stroke:#636379;stroke-width:1;";

    //    <line x1="0" y1="0" x2="200" y2="200" style="stroke:rgb(255,0,0);stroke-width:2"/>


    // TODO: svg-line css style

    return xml + "style=\"" + style + "\" />";

}

std::string SvgRect::dump() const {
    /// basic attributes of svg-rect
    std::string xml = "<rect ";
    xml += "x=\"" + std::to_string(start.x) + "\" ";
    xml += "y=\"" + std::to_string(start.y) + "\" ";
    xml += "width=\"" + std::to_string(width) + "\" ";
    xml += "height=\"" + std::to_string(height) + "\" ";
    if (radius != 0) {
        xml += "rx=\"" + std::to_string(radius) + "\" ";
    }
    /// style attribute of svg-rect
    std::string style = "stroke-width:" + std::to_string(stroke).substr(0, 3) + ";";
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
