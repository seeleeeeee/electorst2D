#ifndef PREPROCESSOR_CORE_H
#define PREPROCESSOR_CORE_H

#include "../../interfaces/ipreprocessor.h"
#include <iostream>
#include <memory>
#include <cmath>

namespace electrostatic {

class BasicPreprocessor : public IPreprocessor {
private:
    Geometry geometry_;
    std::shared_ptr<Mesh> mesh_;
    
public:
    BasicPreprocessor() = default;
    
    void setGeometry(const Geometry& geometry) override {
        geometry_ = geometry;
        std::cout << "[preprocessor] geometry set" << std::endl;
    }
    
    std::shared_ptr<Mesh> generateMesh(double elementSize) override {
        mesh_ = std::make_shared<Mesh>();
        
        // Создаем простую сетку для теста
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                mesh_->nodes.push_back(Node(i*20 + j, -500 + i*50, -500 + j*50));
            }
        }
        
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                int n0 = i*20 + j;
                int n1 = i*20 + j + 1;
                int n2 = (i+1)*20 + j;
                int n3 = (i+1)*20 + j + 1;
                mesh_->elements.push_back(Element(mesh_->elements.size(), 1, {n0, n1, n2}));
                mesh_->elements.push_back(Element(mesh_->elements.size(), 1, {n1, n3, n2}));
            }
        }
        
        std::cout << "[preprocessor] mesh generated: " << mesh_->nodes.size() << " nodes, " 
                  << mesh_->elements.size() << " elements" << std::endl;
        return mesh_;
    }
    
    std::shared_ptr<Mesh> getMesh() const override { return mesh_; }
    const Geometry& getGeometry() const override { return geometry_; }
    void clear() override { mesh_.reset(); }
};

inline std::unique_ptr<IPreprocessor> createPreprocessor() {
    return std::make_unique<BasicPreprocessor>();
}

} // namespace electrostatic

#endif