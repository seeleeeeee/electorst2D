#ifndef MESHER_H
#define MESHER_H

#include "../structures/electrostatic_structures.h"

class Mesher {
public:
    electrostatic::Mesh buildMesh(const electrostatic::Geometry& geometry, double step, bool adaptive = false);
};

#endif