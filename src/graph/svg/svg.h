#pragma once

#include <string>
#include <cstdint>
#include <vector>

/// coordinate point
class Point {
public:
    uint64_t x;
    uint64_t y;
};

/// SVG basic element
class SvgObject {
public:
    virtual ~SvgObject() = default;
    virtual std::string dump() const = 0;
};

// TODO: SVG text element

/// SVG line element
class SvgLine : public SvgObject {
public:
    Point start, end;
    std::string color;
    float width = 1.0;
    float opacity = 0;
    std::string dasharray;

    ~SvgLine() override = default;
    std::string dump() const override;
    SvgLine(Point s, Point e) : start(s), end(e) {}
};

/// SVG rectangle element
class SvgRect : public SvgObject {
public:
    Point start;
    uint64_t width;
    uint64_t height;
    float stroke = 0;
    uint64_t radius = 0;

    std::string color;
    std::string line_color;

    float opacity = 0;
    float line_opacity = 0;

    ~SvgRect() override = default;
    std::string dump() const override;
    SvgRect(Point p, uint64_t w, uint64_t h) : start(p), width(w), height(h) {}
};

/// SVG graph with multi element
class SvgGraph {
public:
    uint64_t width;
    uint64_t height;

    ~SvgGraph();
    std::string dump() const;
    void insert(SvgObject *obj);
    SvgGraph(uint64_t w, uint64_t h) : width(w), height(h) {}

private:
    std::vector<SvgObject*> objects;
};
