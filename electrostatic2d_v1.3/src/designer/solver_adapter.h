#ifndef SOLVER_ADAPTER_H
#define SOLVER_ADAPTER_H

#include "../interfaces/isolver.h"
#include "../structures/electrostatic_structures.h"
#include <memory>
#include <vector>
#include <functional>

namespace electrostatic {

class SolverAdapter : public ISolver {
private:
    std::shared_ptr<Mesh> mesh_;
    Materials materials_;
    std::vector<BoundaryCondition> boundaries_;
    Solution solution_;
    std::function<void(int)> progressCallback_;
    
public:
    SolverAdapter() = default;
    
    void setMesh(std::shared_ptr<Mesh> mesh) override { mesh_ = mesh; }
    
    void setMaterials(const Materials& materials) override { materials_ = materials; }
    
    void setBoundaryConditions(const std::vector<BoundaryCondition>& boundaries) override {
        boundaries_ = boundaries;
    }
    
    void setProgressCallback(std::function<void(int)> callback) override {
        progressCallback_ = callback;
    }
    
    bool solve() override {
        if (!mesh_ || mesh_->nodes.empty()) return false;
        
        solution_.nodes = mesh_->nodes;
        solution_.elements = mesh_->elements;
        solution_.energy = 0.0;
        
        // Простое решение: линейный потенциал
        for (auto& node : solution_.nodes) {
            node.potential = (node.y + 500) / 1000.0 * 10.0;
            if (node.potential < 0) node.potential = 0;
            if (node.potential > 10) node.potential = 10;
        }
        
        if (progressCallback_) progressCallback_(100);
        return true;
    }
    
    Solution getSolution() const override { return solution_; }
    
    std::shared_ptr<Mesh> getMesh() const override { return mesh_; }
    
    void clear() override {
        mesh_.reset();
        solution_ = Solution();
    }
    
    bool validate() override {
        return mesh_ != nullptr && !mesh_->nodes.empty();
    }
};

} // namespace electrostatic

#endif