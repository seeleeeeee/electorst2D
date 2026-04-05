#ifndef ELECTROSTATIC_STRUCTURES_H
#define ELECTROSTATIC_STRUCTURES_H

#include <vector>
#include <string>
#include <memory>

namespace electrostatic {

struct Node {
    int id;
    double x;
    double y;
    double potential;
    Node() : id(-1), x(0), y(0), potential(0) {}
    Node(int i, double xx, double yy) : id(i), x(xx), y(yy), potential(0) {}
};

struct Element {
    int id;
    int type;
    std::vector<int> nodeIds;
    Element() : id(-1), type(1) {}
    Element(int i, int t, const std::vector<int>& ids) : id(i), type(t), nodeIds(ids) {}
};

struct Mesh {
    std::vector<Node> nodes;
    std::vector<Element> elements;
    void clear() { nodes.clear(); elements.clear(); }
    bool isEmpty() const { return nodes.empty(); }
};

struct Dielectric {
    std::string id;
    double erReal;
    std::string name;
    Dielectric() : erReal(1) {}
    Dielectric(const std::string& i, double er, const std::string& n) : id(i), erReal(er), name(n) {}
};

struct Conductor {
    std::string id;
    double conductivity;
    std::string name;
    Conductor() : conductivity(0) {}
    Conductor(const std::string& i, double sigma, const std::string& n) : id(i), conductivity(sigma), name(n) {}
};

struct Materials {
    std::vector<Dielectric> dielectrics;
    std::vector<Conductor> conductors;
};

struct Layer {
    std::string id;
    std::string name;
    int order;
    bool isGround;
    std::string materialId;
    double thickness;
    double width;
    double positionX;
    double positionY;
    bool enabled;
    Layer() : order(0), isGround(false), thickness(1), width(1), positionX(0), positionY(0), enabled(true) {}
};

struct Strip {
    std::string stripId;
    std::string name;
    std::string layerId;
    bool ground;
    std::string materialId;
    double thickness;
    double width;
    double spacing;
    double positionY;
    int edgesType;
    int edgesAngle;
    double edgesRadius;
    Strip() : ground(false), thickness(0), width(0), spacing(0), positionY(0), edgesType(0), edgesAngle(0), edgesRadius(0) {}
};

struct Geometry {
    std::vector<Layer> layers;
    std::vector<Strip> strips;
    Materials materials;
    double width;
    double height;
    Geometry() : width(1000), height(1000) {}
};

struct Solution {
    std::vector<Node> nodes;
    std::vector<Element> elements;
    double energy;
    Solution() : energy(0) {}
};

enum class BoundaryType { Dirichlet, Neumann };

struct BoundaryCondition {
    std::string name;
    BoundaryType type;
    double value;
    std::string region;
    BoundaryCondition() : type(BoundaryType::Dirichlet), value(0) {}
    BoundaryCondition(const std::string& n, BoundaryType t, double v, const std::string& r)
        : name(n), type(t), value(v), region(r) {}
};

struct ColorMap {
    double minValue;
    double maxValue;
    ColorMap() : minValue(0), maxValue(1) {}
};

static const std::vector<Dielectric> DIELECTRICS = {
    Dielectric("air", 1.0, "Воздух"),
    Dielectric("fr4", 4.3, "FR4"),
    Dielectric("teflon", 2.1, "Тефлон"),
    Dielectric("alumina", 9.8, "Алюминий"),
    Dielectric("silicon", 11.7, "Кремний"),
    Dielectric("glass", 7.0, "Стекло")
};

static const std::vector<Conductor> CONDUCTORS = {
    Conductor("copper", 5.96e7, "Медь"),
    Conductor("aluminum", 3.5e7, "Алюминий"),
    Conductor("gold", 4.1e7, "Золото"),
    Conductor("silver", 6.3e7, "Серебро"),
    Conductor("iron", 1.0e7, "Железо")
};

} // namespace electrostatic

#endif