#ifndef FEM2DSOLVER_H
#define FEM2DSOLVER_H

#include "simulationdata.h"
#include "../structures/electrostatic_structures.h"
#include <Eigen/Dense>
#include <unordered_map>
#include <string>

class Fem2DSolver {
public:
    SimulationData prepareSimulationData(
        const electrostatic::Geometry& geo, 
        const electrostatic::Mesh& mesh, 
        double meshStep, 
        bool openBoundary, 
        const std::unordered_map<std::string, double>& voltages) const;
    
    bool computeLineParameters(
        const electrostatic::Geometry& geo, 
        const electrostatic::Mesh& mesh, 
        double meshStep, 
        Eigen::MatrixXd& C, 
        Eigen::MatrixXd& L) const;
};

#endif