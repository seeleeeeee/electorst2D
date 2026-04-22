#ifndef ELECTROSTATIC_STRUCTURES_H
#define ELECTROSTATIC_STRUCTURES_H

#include <vector>
#include <string>
#include <memory>
#include <QColor>

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
    int materialId;
    Element() : id(-1), type(1), materialId(0) {}
    Element(int i, int t, const std::vector<int>& ids) : id(i), type(t), nodeIds(ids), materialId(0) {}
};

struct Mesh {
    std::vector<Node> nodes;
    std::vector<Element> elements;
    void clear() { nodes.clear(); elements.clear(); }
    bool isEmpty() const { return nodes.empty(); }
};

// СТРУКТУРА ПОЛИГОНА ДЛЯ v1.3
struct Polygon {
    std::string id;
    std::string name;
    std::vector<int> pointIds;
    std::string materialId;
    double epsilon;
    QColor color;
    bool isSelected;
    
    Polygon() : epsilon(1.0), color(100, 100, 255), isSelected(false) {}
    
    Polygon(const std::string& i, const std::string& n, const std::vector<int>& pIds,
            const std::string& mId, double eps, const QColor& col)
        : id(i), name(n), pointIds(pIds), materialId(mId), epsilon(eps), color(col), isSelected(false) {}
};

struct Material {
    std::string id;
    std::string name;
    double epsilon;
    QColor color;
    Material() : epsilon(1.0), color(100, 100, 255) {}
    Material(const std::string& i, const std::string& n, double eps, const QColor& col)
        : id(i), name(n), epsilon(eps), color(col) {}
};

static const std::vector<Material> DEFAULT_MATERIALS = {
    Material("air",      "Air",      1.0,  QColor(100, 100, 255)),
    Material("fr4",      "FR4",      4.3,  QColor(100, 150, 200)),
    Material("teflon",   "Teflon",   2.1,  QColor(80, 180, 200)),
    Material("alumina",  "Alumina",  9.8,  QColor(200, 150, 100)),
    Material("silicon",  "Silicon", 11.7,  QColor(255, 100, 100)),
    Material("glass",    "Glass",    7.0,  QColor(180, 180, 100))
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

struct Geometry {
    Materials materials;
    double width;
    double height;
    Geometry() : width(1000), height(1000) {}
};

struct Solution {
    std::vector<Node> nodes;
    std::vector<Element> elements;
    double energy;
    double minPotential;
    double maxPotential;
    
    Solution() : energy(0), minPotential(0), maxPotential(0) {}
    
    void recalculateRange() {
        if (nodes.empty()) {
            minPotential = maxPotential = 0;
            return;
        }
        minPotential = nodes[0].potential;
        maxPotential = nodes[0].potential;
        for (const auto& node : nodes) {
            if (node.potential < minPotential) minPotential = node.potential;
            if (node.potential > maxPotential) maxPotential = node.potential;
        }
    }
    
    void clear() {
        nodes.clear();
        elements.clear();
        energy = 0;
        minPotential = 0;
        maxPotential = 0;
    }
};

struct ColorMap {
    double minValue;
    double maxValue;
    ColorMap() : minValue(0), maxValue(1) {}
};

static const std::vector<Dielectric> DIELECTRICS = {
    Dielectric("air", 1.0, "Air"),
    Dielectric("fr4", 4.3, "FR4"),
    Dielectric("teflon", 2.1, "Teflon"),
    Dielectric("alumina", 9.8, "Alumina"),
    Dielectric("silicon", 11.7, "Silicon"),
    Dielectric("glass", 7.0, "Glass")
};

static const std::vector<Conductor> CONDUCTORS = {
    Conductor("copper", 5.96e7, "Copper"),
    Conductor("aluminum", 3.5e7, "Aluminum"),
    Conductor("gold", 4.1e7, "Gold"),
    Conductor("silver", 6.3e7, "Silver"),
    Conductor("iron", 1.0e7, "Iron")
};

} // namespace electrostatic

#endif