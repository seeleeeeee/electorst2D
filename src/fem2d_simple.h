#ifndef FEM2D_SIMPLE_H
#define FEM2D_SIMPLE_H

#include "structures/electrostatic_structures.h"
#include <vector>
#include <cmath>

namespace electrostatic {

class SimpleSolver {
public:
    static std::shared_ptr<Mesh> generateMesh(const Geometry& geo, double elementSize) {
        auto mesh = std::make_shared<Mesh>();
        
        // Создаем сетку 20x20
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                mesh->nodes.push_back(Node(i*20 + j, -500 + i*50, -500 + j*50));
            }
        }
        
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                int n0 = i*20 + j;
                int n1 = i*20 + j + 1;
                int n2 = (i+1)*20 + j;
                int n3 = (i+1)*20 + j + 1;
                mesh->elements.push_back(Element(mesh->elements.size(), 1, {n0, n1, n2}));
                mesh->elements.push_back(Element(mesh->elements.size(), 1, {n1, n3, n2}));
            }
        }
        
        return mesh;
    }
    
    static Solution solve(std::shared_ptr<Mesh> mesh) {
        Solution sol;
        sol.nodes = mesh->nodes;
        sol.elements = mesh->elements;
        sol.energy = 0;
        
        // Линейный потенциал
        for (auto& node : sol.nodes) {
            node.potential = (node.y + 500) / 1000.0 * 10.0;
            if (node.potential < 0) node.potential = 0;
            if (node.potential > 10) node.potential = 10;
        }
        
        return sol;
    }
};

} // namespace electrostatic

#endif