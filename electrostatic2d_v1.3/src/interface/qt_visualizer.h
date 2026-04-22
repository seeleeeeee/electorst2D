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
#include <cmath>
#include <algorithm>
#include <iostream>
#include "../structures/electrostatic_structures.h"

namespace electrostatic {

class QtVisualizer {
private:
    QGraphicsScene* scene_;
    
    QColor getColorForValue(double value, double minVal, double maxVal) {
        if (maxVal <= minVal) return QColor(0, 0, 255);
        
        double t = (value - minVal) / (maxVal - minVal);
        t = std::max(0.0, std::min(1.0, t));
        
        int r, g, b;
        if (t < 0.2) {
            r = 0;
            g = 0;
            b = 255;
        } else if (t < 0.4) {
            double p = (t - 0.2) / 0.2;
            r = 0;
            g = (int)(255 * p);
            b = 255;
        } else if (t < 0.6) {
            double p = (t - 0.4) / 0.2;
            r = 0;
            g = 255;
            b = (int)(255 * (1 - p));
        } else if (t < 0.8) {
            double p = (t - 0.6) / 0.2;
            r = (int)(255 * p);
            g = 255;
            b = 0;
        } else {
            r = 255;
            g = (int)(255 * (1 - (t - 0.8) / 0.2));
            b = 0;
        }
        
        return QColor(r, g, b);
    }
    
public:
    QtVisualizer(QGraphicsScene* scene = nullptr) : scene_(scene) {}
    
    void setScene(QGraphicsScene* scene) { scene_ = scene; }
    
    void clear() { 
        if (scene_) scene_->clear(); 
    }
    
    void drawMesh(std::shared_ptr<Mesh> mesh) {
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
                    scene_->addPolygon(polygon, pen, QBrush(QColor(200, 200, 200, 80)));
                }
            }
        }
        
        QPen nodePen(Qt::red, 2);
        QBrush nodeBrush(Qt::red);
        for (const auto& node : mesh->nodes) {
            scene_->addEllipse(node.x - 3, node.y - 3, 6, 6, nodePen, nodeBrush);
        }
    }
    
    void drawPotential(std::shared_ptr<Solution> solution, ColorMap colormap) {
        if (!scene_ || !solution) return;
        
        std::cout << "[Visualizer] drawPotential: " << solution->elements.size() 
                  << " elements, range [" << colormap.minValue << ", " << colormap.maxValue << "]" << std::endl;
        
        for (const auto& elem : solution->elements) {
            if (elem.nodeIds.size() >= 3) {
                QPolygonF polygon;
                double avgPotential = 0.0;
                int validNodes = 0;
                for (int nodeId : elem.nodeIds) {
                    if (nodeId >= 0 && nodeId < (int)solution->nodes.size()) {
                        polygon << QPointF(solution->nodes[nodeId].x, solution->nodes[nodeId].y);
                        avgPotential += solution->nodes[nodeId].potential;
                        validNodes++;
                    }
                }
                if (polygon.size() >= 3 && validNodes > 0) {
                    avgPotential /= validNodes;
                    QColor color = getColorForValue(avgPotential, colormap.minValue, colormap.maxValue);
                    color.setAlpha(200);
                    scene_->addPolygon(polygon, QPen(QColor(100, 100, 100), 1), QBrush(color));
                }
            }
        }
        
        QPen contourPen(QColor(80, 80, 80), 1);
        for (const auto& elem : solution->elements) {
            if (elem.nodeIds.size() >= 3) {
                QPolygonF polygon;
                for (int nodeId : elem.nodeIds) {
                    if (nodeId >= 0 && nodeId < (int)solution->nodes.size()) {
                        polygon << QPointF(solution->nodes[nodeId].x, solution->nodes[nodeId].y);
                    }
                }
                if (polygon.size() >= 3) {
                    scene_->addPolygon(polygon, contourPen, Qt::NoBrush);
                }
            }
        }
    }
    
    void redrawAll(std::shared_ptr<Mesh> mesh, std::shared_ptr<Solution> solution, ColorMap colormap) {
        std::cout << "[Visualizer] redrawAll called" << std::endl;
        clear();
        drawMesh(mesh);
        drawPotential(solution, colormap);
        if (scene_) {
            scene_->update();
        }
    }
};

} // namespace electrostatic

#endif