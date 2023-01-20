#pragma once

#include <string>
#include <cstdint>
#include <vector>

class SvgObject {
public:
    virtual ~SvgObject() = default;
    virtual std::string dump() const = 0;
};

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

class SvgLine : public SvgObject {
public:
    uint64_t start_x;
    uint64_t start_y;

    uint64_t end_x;
    uint64_t end_y;

    // TODO: more options for svg-line

    ~SvgLine() override = default;
    std::string dump() const override;

    // TODO: SvgLine(...)
};

class SvgRect : public SvgObject {
public:
    uint64_t top;
    uint64_t left;
    uint64_t width;
    uint64_t height;

    uint64_t radius = 0;
    uint32_t stroke = 1;

    std::string color;
    std::string line_color;

    float opacity = 0;
    float line_opacity = 0;

    ~SvgRect() override = default;
    std::string dump() const override;
    SvgRect(uint64_t t, uint64_t l, uint64_t w, uint64_t h) : top(t), left(l), width(w), height(h) {}
};
