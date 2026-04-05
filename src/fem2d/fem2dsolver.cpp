#include "fem2dsolver.h"

SimulationData Fem2DSolver::prepareSimulationData(
    const electrostatic::Geometry& geo, 
    const electrostatic::Mesh& mesh, 
    double meshStep, 
    bool openBoundary, 
    const std::unordered_map<std::string, double>& voltages) const 
{
    return SimulationData();
}

bool Fem2DSolver::computeLineParameters(
    const electrostatic::Geometry& geo, 
    const electrostatic::Mesh& mesh, 
    double meshStep, 
    Eigen::MatrixXd& C, 
    Eigen::MatrixXd& L) const 
{
    C.resize(1,1);
    L.resize(1,1);
    C(0,0) = 1.0;
    L(0,0) = 1.0;
    return true;
}