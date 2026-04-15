#ifndef GMSH_MESHER_H
#define GMSH_MESHER_H

#include "../../structures/electrostatic_structures.h"
#include "../../interfaces/ipreprocessor.h"
#include <memory>
#include <vector>
#include <QPointF>
#include <map>

namespace electrostatic {

class GmshMesher : public IPreprocessor {
private:
    Geometry geometry_;
    std::shared_ptr<Mesh> mesh_;
    double elementSize_;
    bool gmshAvailable_;
    
    std::string modelName_;
    std::map<int, int> pointTags_;
    std::map<std::pair<int, int>, int> lineTags_;
    std::map<int, int> surfaceTags_;
    
public:
    GmshMesher();
    ~GmshMesher();
    
    void setGeometry(const Geometry& geometry) override;
    std::shared_ptr<Mesh> generateMesh(double elementSize) override;
    std::shared_ptr<Mesh> getMesh() const override;
    const Geometry& getGeometry() const override;
    void clear() override;
    
    bool isAvailable() const { return gmshAvailable_; }
    
    // Метод для генерации простой сетки (fallback)
    std::shared_ptr<Mesh> generateSimpleMesh();
    
    std::shared_ptr<Mesh> generateMeshFromPolygons(const std::vector<Polygon>& polygons, double elementSize);
    bool exportToFile(const QString& filename);
    bool importFromFile(const QString& filename);
    
private:
    bool initializeGmsh();
    void finalizeGmsh();
    bool createGeometryFromPolygons(const std::vector<Polygon>& polygons);
    bool createGeometryFromPointsAndEdges(const std::vector<QPointF>& points, 
                                           const std::vector<std::pair<int, int>>& edges);
    bool generateGmshMesh();
    bool extractMesh();
};

} // namespace electrostatic

#endif