#include "svg.h"

inline std::string float_to_string(float data) {
    auto str = std::to_string(data);
    return str.substr(0, str.find('.') + 3); // format -> xxx.__
}

/// SvgGraph interface
void SvgGraph::insert(SvgObject *obj) {
    objects.emplace_back(obj);
}

SvgGraph::~SvgGraph() {
    for (auto *object : objects) {
        delete object;
    }
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

/// SvgLine render as XML content
std::string SvgLine::dump() const {
    /// basic attributes of svg-line
    std::string xml = "<line ";
    xml += "x1=\"" + std::to_string(start.x) + "\" ";
    xml += "y1=\"" + std::to_string(start.y) + "\" ";
    xml += "x2=\"" + std::to_string(end.x) + "\" ";
    xml += "y2=\"" + std::to_string(end.y) + "\" ";
    if (!dasharray.empty()) {
        xml += "stroke-dasharray=\"" + dasharray + "\" ";
    }

    /// style attribute of svg-line
    std::string style = "stroke-width:" + float_to_string(width) + ";";
    if (!color.empty()) {
        style += "stroke:" + color + ";";
    }
    if (opacity != 0) {
        style += "stroke-opacity:" + float_to_string(opacity) + ";";
    }
    return xml + "style=\"" + style + "\" />";
}

/// SvgRect render as XML content
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
    std::string style = "stroke-width:" + float_to_string(stroke) + ";";
    if (!color.empty()) {
        style += "fill:" + color + ";";
    }
    if (!line_color.empty()) {
        style += "stroke:" + line_color + ";";
    }
    if (opacity != 0) {
        style += "fill-opacity:" + float_to_string(opacity) + ";";
    }
    if (line_opacity != 0) {
        style += "stroke-opacity:" + float_to_string(line_opacity) + ";";
    }
    return xml + "style=\"" + style + "\" />";
}
