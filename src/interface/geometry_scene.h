#ifndef GEOMETRY_SCENE_H
#define GEOMETRY_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QVector>
#include <QPointF>
#include <QDebug>

class GeometryScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { SelectMode, AddNodeMode, AddEdgeMode };

    explicit GeometryScene(QObject *parent = nullptr);
    
    void setMode(Mode mode);
    Mode getMode() const { return currentMode; }
    void setGridEnabled(bool enabled);
    void setGridSize(double size);
    void clearAll();
    
    QVector<QGraphicsEllipseItem*> getNodes() const { return nodes; }
    QVector<QGraphicsLineItem*> getEdges() const { return edges; }
    QVector<QPointF> getPolygon() const;

signals:
    void statusMessage(const QString& msg);
    void nodeAdded(const QPointF& pos);
    void edgeAdded(const QPointF& p1, const QPointF& p2);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    Mode currentMode;
    bool gridEnabled;
    double gridSize;
    
    QPointF snapToGridPoint(const QPointF& point);
    QGraphicsEllipseItem* createNode(const QPointF& pos);
    QGraphicsLineItem* createEdge(const QPointF& p1, const QPointF& p2);
    void updateHoverNode(const QPointF& pos);
    
    QVector<QGraphicsEllipseItem*> nodes;
    QVector<QGraphicsLineItem*> edges;
    
    QGraphicsLineItem* previewEdge;
    QGraphicsEllipseItem* startNode;
    QGraphicsEllipseItem* hoverNode;
    bool drawing;
    bool isDragging;
};

#endif