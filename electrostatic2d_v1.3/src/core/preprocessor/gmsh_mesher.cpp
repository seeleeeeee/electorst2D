#include "gmsh_mesher.h"
#include <iostream>

namespace electrostatic {

// Правильный порядок инициализации: сначала gmshAvailable_, потом elementSize_
GmshMesher::GmshMesher() : elementSize_(0.1), gmshAvailable_(false)
{
    std::cout << "[GmshMesher] Using fallback mode" << std::endl;
}

GmshMesher::~GmshMesher() {}

void GmshMesher::setGeometry(const Geometry& geometry) 
{ 
    geometry_ = geometry; 
}

std::shared_ptr<Mesh> GmshMesher::generateMesh(double elementSize)
{
    elementSize_ = elementSize;
    return generateSimpleMesh();
}

std::shared_ptr<Mesh> GmshMesher::generateSimpleMesh()
{
    auto mesh = std::make_shared<Mesh>();
    
    int resolution = 30;
    double size = 1000.0;
    double start = -size/2;
    
    for (int i = 0; i <= resolution; i++) {
        for (int j = 0; j <= resolution; j++) {
            double x = start + size * i / resolution;
            double y = start + size * j / resolution;
            mesh->nodes.push_back(Node(mesh->nodes.size(), x, y));
        }
    }
    
    int elemId = 0;
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            int n0 = i * (resolution + 1) + j;
            int n1 = i * (resolution + 1) + j + 1;
            int n2 = (i + 1) * (resolution + 1) + j;
            int n3 = (i + 1) * (resolution + 1) + j + 1;
            
            mesh->elements.push_back(Element(elemId++, 2, {n0, n1, n2}));
            mesh->elements.push_back(Element(elemId++, 2, {n1, n3, n2}));
        }
    }
    
    std::cout << "[GmshMesher] Simple mesh: " << mesh->nodes.size() 
              << " nodes, " << mesh->elements.size() << " elements" << std::endl;
    
    mesh_ = mesh;
    return mesh;
}

std::shared_ptr<Mesh> GmshMesher::generateMeshFromPolygons(const std::vector<Polygon>& polygons, double elementSize)
{
    (void)polygons;
    return generateMesh(elementSize);
}

std::shared_ptr<Mesh> GmshMesher::getMesh() const 
{ 
    return mesh_; 
}

const Geometry& GmshMesher::getGeometry() const 
{ 
    return geometry_; 
}

void GmshMesher::clear() 
{ 
    mesh_.reset(); 
}

bool GmshMesher::exportToFile(const QString& filename) 
{ 
    (void)filename; 
    return false; 
}

bool GmshMesher::importFromFile(const QString& filename) 
{ 
    (void)filename; 
    return false; 
}

bool GmshMesher::initializeGmsh() 
{ 
    return false; 
}

void GmshMesher::finalizeGmsh() {}

bool GmshMesher::createGeometryFromPolygons(const std::vector<Polygon>& polygons) 
{ 
    (void)polygons; 
    return false; 
}

bool GmshMesher::createGeometryFromPointsAndEdges(const std::vector<QPointF>& points, 
                                                   const std::vector<std::pair<int, int>>& edges) 
{ 
    (void)points; 
    (void)edges; 
    return false; 
}

bool GmshMesher::generateGmshMesh() 
{ 
    return false; 
}

bool GmshMesher::extractMesh() 
{ 
    return false; 
}

} // namespace electrostatic