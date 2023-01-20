#pragma once

#include <string>
#include <cstdint>
#include <vector>

class SvgObject {
public:
    virtual ~SvgObject() = default;
    virtual std::string dump() const = 0;
};

class SvgLine : public SvgObject {
public:
    uint64_t start_x;
    uint64_t start_y;

    uint64_t end_x;
    uint64_t end_y;

    ~SvgLine() override = default;
    std::string dump() const override;

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
};

class SvgGraph {
public:

    std::vector<SvgObject*> objects;

//    void insert(SvgObject *obj);

    void insert(const SvgRect &rect);
    void insert(const SvgLine &line);

//    void insert(SvgRect rect);
//    void insert(SvgLine line);


    std::string dump() const;

    ~SvgGraph();

};
