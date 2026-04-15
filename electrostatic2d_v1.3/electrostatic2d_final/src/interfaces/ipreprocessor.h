#ifndef IPREPROCESSOR_H
#define IPREPROCESSOR_H

#include "../structures/electrostatic_structures.h"
#include <memory>

namespace electrostatic {

class IPreprocessor {
public:
    virtual ~IPreprocessor() = default;
    virtual void setGeometry(const Geometry& geometry) = 0;
    virtual std::shared_ptr<Mesh> generateMesh(double elementSize) = 0;
    virtual std::shared_ptr<Mesh> getMesh() const = 0;
    virtual const Geometry& getGeometry() const = 0;
    virtual void clear() = 0;
};

} // namespace electrostatic

#endif