#ifndef MATERIALS_DATA_H
#define MATERIALS_DATA_H

#include "../structures/electrostatic_structures.h"
#include <vector>

namespace electrostatic {

static const std::vector<Dielectric> DIELECTRICS = {
    Dielectric("air", 1.0, "Воздух"),
    Dielectric("fr4", 4.3, "FR4"),
    Dielectric("teflon", 2.1, "Тефлон"),
    Dielectric("alumina", 9.8, "Алюминий"),
    Dielectric("silicon", 11.7, "Кремний"),
    Dielectric("glass", 7.0, "Стекло")
};

static const std::vector<Conductor> CONDUCTORS = {
    Conductor("copper", 5.96e7, "Медь"),
    Conductor("aluminum", 3.5e7, "Алюминий"),
    Conductor("gold", 4.1e7, "Золото"),
    Conductor("silver", 6.3e7, "Серебро"),
    Conductor("iron", 1.0e7, "Железо")
};

} // namespace electrostatic

#endif