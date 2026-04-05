#ifndef SOLVERAPI_H
#define SOLVERAPI_H

#include <string>
#include <memory>

class SolverAPI {
public:
    SolverAPI();
    ~SolverAPI();
    bool loadGeometry(const std::string& source, bool isFile);
    bool buildMesh(double step, bool adaptive = false);
};

#endif