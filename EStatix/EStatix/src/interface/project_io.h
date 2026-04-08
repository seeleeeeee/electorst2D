#ifndef PROJECT_IO_H
#define PROJECT_IO_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPointF>
#include "geometry_scene.h"
#include "../structures/electrostatic_structures.h"

class ProjectIO {
public:
    // Сохранение всего проекта (геометрия + сетка + решение)
    static bool saveFullProject(const QString& filename, GeometryScene* scene,
                                 std::shared_ptr<electrostatic::Mesh> mesh,
                                 const electrostatic::Solution& solution) {
        QJsonObject root;
        
        // 1. Сохраняем геометрию (точки и линии)
        QJsonArray nodesArray;
        for (auto node : scene->getNodes()) {
            QJsonObject nodeObj;
            nodeObj["x"] = node->pos().x();
            nodeObj["y"] = node->pos().y();
            nodesArray.append(nodeObj);
        }
        root["geometry_nodes"] = nodesArray;
        
        QJsonArray edgesArray;
        for (auto edge : scene->getEdges()) {
            QJsonObject edgeObj;
            QLineF line = edge->line();
            edgeObj["x1"] = line.x1();
            edgeObj["y1"] = line.y1();
            edgeObj["x2"] = line.x2();
            edgeObj["y2"] = line.y2();
            edgesArray.append(edgeObj);
        }
        root["geometry_edges"] = edgesArray;
        
        // 2. Сохраняем сетку (узлы и элементы)
        if (mesh && !mesh->nodes.empty()) {
            QJsonArray meshNodesArray;
            for (const auto& node : mesh->nodes) {
                QJsonObject nodeObj;
                nodeObj["id"] = node.id;
                nodeObj["x"] = node.x;
                nodeObj["y"] = node.y;
                meshNodesArray.append(nodeObj);
            }
            root["mesh_nodes"] = meshNodesArray;
            
            QJsonArray elementsArray;
            for (const auto& elem : mesh->elements) {
                QJsonObject elemObj;
                elemObj["id"] = elem.id;
                elemObj["type"] = elem.type;
                QJsonArray idsArray;
                for (int id : elem.nodeIds) {
                    idsArray.append(id);
                }
                elemObj["nodeIds"] = idsArray;
                elementsArray.append(elemObj);
            }
            root["mesh_elements"] = elementsArray;
        }
        
        // 3. Сохраняем решение (потенциалы)
        if (!solution.nodes.empty()) {
            QJsonArray solutionArray;
            for (const auto& node : solution.nodes) {
                QJsonObject nodeObj;
                nodeObj["id"] = node.id;
                nodeObj["potential"] = node.potential;
                solutionArray.append(nodeObj);
            }
            root["solution_potentials"] = solutionArray;
            root["solution_energy"] = solution.energy;
        }
        
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) return false;
        file.write(QJsonDocument(root).toJson());
        return true;
    }
    
    // Загрузка полного проекта
    static bool loadFullProject(const QString& filename, GeometryScene* scene,
                                 std::shared_ptr<electrostatic::Mesh>& mesh,
                                 electrostatic::Solution& solution) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return false;
        
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();
        
        scene->clearAll();
        
        // Загружаем геометрию (точки)
        QJsonArray nodesArray = root["geometry_nodes"].toArray();
        for (const auto& nodeVal : nodesArray) {
            QJsonObject nodeObj = nodeVal.toObject();
            double x = nodeObj["x"].toDouble();
            double y = nodeObj["y"].toDouble();
            
            QGraphicsEllipseItem* node = scene->addEllipse(-5, -5, 10, 10, QPen(Qt::black, 2), QBrush(Qt::white));
            node->setPos(x, y);
            node->setFlag(QGraphicsItem::ItemIsMovable);
        }
        
        // Загружаем линии
        QJsonArray edgesArray = root["geometry_edges"].toArray();
        for (const auto& edgeVal : edgesArray) {
            QJsonObject edgeObj = edgeVal.toObject();
            double x1 = edgeObj["x1"].toDouble();
            double y1 = edgeObj["y1"].toDouble();
            double x2 = edgeObj["x2"].toDouble();
            double y2 = edgeObj["y2"].toDouble();
            scene->addLine(x1, y1, x2, y2, QPen(Qt::black, 2));
        }
        
        // Загружаем сетку
        if (root.contains("mesh_nodes")) {
            mesh = std::make_shared<electrostatic::Mesh>();
            
            QJsonArray meshNodesArray = root["mesh_nodes"].toArray();
            for (const auto& nodeVal : meshNodesArray) {
                QJsonObject nodeObj = nodeVal.toObject();
                electrostatic::Node node;
                node.id = nodeObj["id"].toInt();
                node.x = nodeObj["x"].toDouble();
                node.y = nodeObj["y"].toDouble();
                mesh->nodes.push_back(node);
            }
            
            QJsonArray elementsArray = root["mesh_elements"].toArray();
            for (const auto& elemVal : elementsArray) {
                QJsonObject elemObj = elemVal.toObject();
                electrostatic::Element elem;
                elem.id = elemObj["id"].toInt();
                elem.type = elemObj["type"].toInt();
                QJsonArray idsArray = elemObj["nodeIds"].toArray();
                for (const auto& idVal : idsArray) {
                    elem.nodeIds.push_back(idVal.toInt());
                }
                mesh->elements.push_back(elem);
            }
        }
        
        // Загружаем решение
        if (root.contains("solution_potentials")) {
            solution.nodes.clear();
            solution.elements.clear();
            
            QJsonArray solutionArray = root["solution_potentials"].toArray();
            for (const auto& nodeVal : solutionArray) {
                QJsonObject nodeObj = nodeVal.toObject();
                electrostatic::Node node;
                node.id = nodeObj["id"].toInt();
                node.potential = nodeObj["potential"].toDouble();
                solution.nodes.push_back(node);
            }
            solution.energy = root["solution_energy"].toDouble();
        }
        
        return true;
    }
    
    // Простое сохранение (только геометрия) для обратной совместимости
    static bool saveProject(const QString& filename, GeometryScene* scene) {
        QJsonObject root;
        
        QJsonArray nodesArray;
        for (auto node : scene->getNodes()) {
            QJsonObject nodeObj;
            nodeObj["x"] = node->pos().x();
            nodeObj["y"] = node->pos().y();
            nodesArray.append(nodeObj);
        }
        root["nodes"] = nodesArray;
        
        QJsonArray edgesArray;
        for (auto edge : scene->getEdges()) {
            QJsonObject edgeObj;
            QLineF line = edge->line();
            edgeObj["x1"] = line.x1();
            edgeObj["y1"] = line.y1();
            edgeObj["x2"] = line.x2();
            edgeObj["y2"] = line.y2();
            edgesArray.append(edgeObj);
        }
        root["edges"] = edgesArray;
        
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) return false;
        file.write(QJsonDocument(root).toJson());
        return true;
    }
    
    static bool loadProject(const QString& filename, GeometryScene* scene) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return false;
        
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject root = doc.object();
        
        scene->clearAll();
        
        QJsonArray nodesArray = root["nodes"].toArray();
        for (const auto& nodeVal : nodesArray) {
            QJsonObject nodeObj = nodeVal.toObject();
            double x = nodeObj["x"].toDouble();
            double y = nodeObj["y"].toDouble();
            
            QGraphicsEllipseItem* node = scene->addEllipse(-5, -5, 10, 10, QPen(Qt::black, 2), QBrush(Qt::white));
            node->setPos(x, y);
            node->setFlag(QGraphicsItem::ItemIsMovable);
        }
        
        QJsonArray edgesArray = root["edges"].toArray();
        for (const auto& edgeVal : edgesArray) {
            QJsonObject edgeObj = edgeVal.toObject();
            double x1 = edgeObj["x1"].toDouble();
            double y1 = edgeObj["y1"].toDouble();
            double x2 = edgeObj["x2"].toDouble();
            double y2 = edgeObj["y2"].toDouble();
            scene->addLine(x1, y1, x2, y2, QPen(Qt::black, 2));
        }
        
        return true;
    }
};

#endif