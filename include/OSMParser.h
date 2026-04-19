#ifndef OSMPARSER_H
#define OSMPARSER_H

#include "MapGraph.h"
#include <string>

class OSMParser {
public:
    // Basic XML parser specific to OSM structure
    // parses <node> and <way> -> <nd> tags
    static bool parseOSMFile(const std::string& filename, MapGraph& graph);
};

#endif
