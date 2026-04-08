#ifndef IVISUALIZER_H
#define IVISUALIZER_H

#include "../structures/electrostatic_structures.h"
#include <memory>
#include <vector>
#include <string>

namespace electrostatic {

class IVisualizer {
public:
    virtual ~IVisualizer() = default;
    virtual void drawMesh(std::shared_ptr<Mesh> mesh) = 0;
    virtual void drawPotential(std::shared_ptr<Solution> solution, ColorMap colormap) = 0;
    virtual void drawField(std::shared_ptr<Solution> solution) = 0;
    virtual void drawFieldLines(std::shared_ptr<Solution> solution, int numLines) = 0;
    virtual void drawBoundaries(const std::vector<BoundaryCondition>& boundaries) = 0;
    virtual void drawLabels(const std::vector<std::string>& labels) = 0;
    virtual void clear() = 0;
    virtual void refresh() = 0;
    virtual bool exportToSVG(const std::string& filename) = 0;
    virtual bool exportToBMP(const std::string& filename) = 0;
    virtual void zoom(double factor) = 0;
    virtual void pan(int dx, int dy) = 0;
    virtual std::pair<int, int> getNodeAt(int x, int y) = 0;
    virtual void printToConsole() = 0;
};

} // namespace electrostatic

#endif