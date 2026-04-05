#include "solverapi.h"

SolverAPI::SolverAPI() {}
SolverAPI::~SolverAPI() {}
bool SolverAPI::loadGeometry(const std::string& source, bool isFile) { return true; }
bool SolverAPI::buildMesh(double step, bool adaptive) { return true; }
bool SolverAPI::calcFields(bool openBoundary, const std::unordered_map<std::string,double>& voltages) { return true; }
bool SolverAPI::saveMesh(const std::string& path) { return true; }
bool SolverAPI::saveFields(const std::string& path) { return true; }
bool SolverAPI::saveLineParameters(const std::string& path, const Eigen::MatrixXd& C, const Eigen::MatrixXd& L) { return true; }
bool SolverAPI::computeLineParameters(Eigen::MatrixXd& C, Eigen::MatrixXd& L) { return true; }