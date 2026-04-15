#ifndef GEOMETRY_SCENE_H
#define GEOMETRY_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QVector>
#include <QPointF>
#include <QDebug>
#include "../structures/electrostatic_structures.h"

class GeometryScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { SelectMode, AddNodeMode, AddEdgeMode };

    explicit GeometryScene(QObject *parent = nullptr);
    ~GeometryScene();
    
    void setMode(Mode mode);
    Mode getMode() const { return currentMode; }
    void setGridEnabled(bool enabled);
    void setGridSize(double size);
    void clearAll();
    
    QVector<QGraphicsEllipseItem*> getNodes() const { return points; }
    QVector<QGraphicsLineItem*> getEdges() const { return lines; }
    QVector<QPointF> getPolygon() const;
    
    // Заглушки для полигонов
    void clearPolygons();
    void setPolygons(const std::vector<electrostatic::Polygon>&);
    void addPolygon(const electrostatic::Polygon&);
    electrostatic::Polygon createPolygonFromAllEdges(const QString& name, const QString& materialId);

signals:
    void statusMessage(const QString& msg);
    // ВРЕМЕННО ОТКЛЮЧАЕМ ЭТОТ СИГНАЛ - ОН ВЫЗЫВАЕТ КРАШ
    // void geometryChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    Mode currentMode;
    bool gridEnabled;
    double gridSize;
    
    QPointF snapToGrid(const QPointF& point);
    QGraphicsEllipseItem* findPointAt(const QPointF& pos, double tolerance);
    QGraphicsEllipseItem* createPoint(const QPointF& pos);
    QGraphicsLineItem* createLine(const QPointF& p1, const QPointF& p2);
    
    QVector<QGraphicsEllipseItem*> points;
    QVector<QGraphicsLineItem*> lines;
    
    QGraphicsLineItem* previewLine;
    QGraphicsEllipseItem* startPoint;
    bool drawing;
};

#endif