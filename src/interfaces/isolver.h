#ifndef ISOLVER_H
#define ISOLVER_H

#include "../structures/electrostatic_structures.h"
#include <memory>
#include <vector>
#include <functional>

namespace electrostatic {

class ISolver {
public:
    virtual ~ISolver() = default;
    virtual void setMesh(std::shared_ptr<Mesh> mesh) = 0;
    virtual void setMaterials(const Materials& materials) = 0;
    virtual void setBoundaryConditions(const std::vector<BoundaryCondition>& boundaries) = 0;
    virtual void setProgressCallback(std::function<void(int)> callback) = 0;
    virtual bool solve() = 0;
    virtual Solution getSolution() const = 0;
    virtual std::shared_ptr<Mesh> getMesh() const = 0;
    virtual void clear() = 0;
    virtual bool validate() = 0;
};

} // namespace electrostatic

#endif