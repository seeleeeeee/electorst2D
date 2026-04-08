#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <vector>
#include <array>
#include <utility>

struct SimulationData {
    std::vector<std::array<double, 2>> p;
    std::vector<std::array<int, 3>> t;
    std::vector<std::pair<int, double>> eps;
    std::vector<std::pair<int, double>> bcs;
};

#endif