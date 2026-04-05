#ifndef GEOMETRYPARSER_H
#define GEOMETRYPARSER_H

#include "../structures/electrostatic_structures.h"
#include <string>

class GeometryParser {
public:
    electrostatic::Geometry parseString(const std::string& jsonText) const;
    electrostatic::Geometry parseFile(const std::string& filePath) const;
};

#endif