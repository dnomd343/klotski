#include "svg.h"

#include <cstdio>

#include <iostream>

std::string SvgRect::dump() const {

    /// <rect x="120" width="100" height="100" rx="15" />

//    char *radius_str = nullptr;
//    if (radius == 0) {
//        asprintf(&radius_str, " ");
//    } else {
//        asprintf(&radius_str, R"( rx="%lu" )", radius);
//    }

    std::string style;

//    style="fill:blue;stroke:pink;stroke-width:5;fill-opacity:0.1;stroke-opacity:0.9"

    char *xml = nullptr;
    asprintf(&xml, R"(<rect x="%ld" y="%ld" width="%ld" height="%ld")", left, top, width, height);
    std::string ret(xml);
    free(xml);

    if (radius != 0) {
        char *radius_str = nullptr;
        asprintf(&radius_str, R"( rx="%lu")", radius);

        ret += radius_str;

        free(radius_str);
    }


//    std::cout << xml << std::endl;

//    std::string result = xml;

//    free(xml);

//    std::string xml("<rect ");
//    xml += "x = ";
//
//    xml += atoi();

    return ret + " />";

}
