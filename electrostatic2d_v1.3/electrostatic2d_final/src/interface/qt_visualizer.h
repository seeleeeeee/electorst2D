#ifndef QT_VISUALIZER_H
#define QT_VISUALIZER_H

#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPolygonF>
#include <QPointF>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include "../structures/electrostatic_structures.h"
#include "../interfaces/ivisualizer.h"

namespace electrostatic {

class QtVisualizer : public IVisualizer {
private:
    QGraphicsScene* scene_;
    double zoomLevel_;
    int panX_, panY_;
    
public:
    QtVisualizer(QGraphicsScene* scene = nullptr) : scene_(scene), zoomLevel_(1.0), panX_(0), panY_(0) {}
    
    void setScene(QGraphicsScene* scene) { scene_ = scene; }
    
    void drawMesh(std::shared_ptr<Mesh> mesh) override {
        if (!scene_ || !mesh) return;
        
        QPen pen(QColor(150, 150, 150), 1);
        
        for (const auto& elem : mesh->elements) {
            if (elem.nodeIds.size() >= 3) {
                QPolygonF polygon;
                for (int nodeId : elem.nodeIds) {
                    if (nodeId >= 0 && nodeId < (int)mesh->nodes.size()) {
                        polygon << QPointF(mesh->nodes[nodeId].x, mesh->nodes[nodeId].y);
                    }
                }
                if (polygon.size() >= 3) {
                    scene_->addPolygon(polygon, pen, QBrush(QColor(200, 200, 200, 50)));
                }
            }
        }
        
        QPen nodePen(Qt::red, 3);
        for (const auto& node : mesh->nodes) {
            scene_->addEllipse(node.x - 2, node.y - 2, 4, 4, nodePen, QBrush(Qt::red));
        }
    }
    
    void drawPotential(std::shared_ptr<Solution> solution, ColorMap colormap) override {
        if (!scene_ || !solution) return;
        (void)colormap;
        
        QPen pen(Qt::blue, 1);
        
        for (const auto& elem : solution->elements) {
            if (elem.nodeIds.size() >= 3) {
                QPolygonF polygon;
                double avgPotential = 0.0;
                for (int nodeId : elem.nodeIds) {
                    if (nodeId >= 0 && nodeId < (int)solution->nodes.size()) {
                        polygon << QPointF(solution->nodes[nodeId].x, solution->nodes[nodeId].y);
                        avgPotential += solution->nodes[nodeId].potential;
                    }
                }
                if (polygon.size() >= 3) {
                    avgPotential /= elem.nodeIds.size();
                    int intensity = (int)(avgPotential / 10.0 * 255);
                    intensity = std::max(0, std::min(255, intensity));
                    QColor color(255 - intensity, 0, intensity);
                    scene_->addPolygon(polygon, pen, QBrush(color));
                }
            }
        }
    }
    
    void drawField(std::shared_ptr<Solution> solution) override { (void)solution; }
    void drawFieldLines(std::shared_ptr<Solution> solution, int numLines) override { (void)solution; (void)numLines; }
    void drawBoundaries(const std::vector<BoundaryCondition>& boundaries) override { (void)boundaries; }
    void drawLabels(const std::vector<std::string>& labels) override { (void)labels; }
    void clear() override { if (scene_) scene_->clear(); }
    void refresh() override { if (scene_) scene_->update(); }
    bool exportToSVG(const std::string& filename) override { (void)filename; return true; }
    bool exportToBMP(const std::string& filename) override { (void)filename; return true; }
    void zoom(double factor) override { zoomLevel_ *= factor; }
    void pan(int dx, int dy) override { panX_ += dx; panY_ += dy; }
    std::pair<int, int> getNodeAt(int x, int y) override { (void)x; (void)y; return std::make_pair(-1, -1); }
    void printToConsole() override {}
};

} // namespace electrostatic

#endif