#ifndef GEOMED_H
#define GEOMED_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QVector>

class GeomEd : public QGraphicsScene {
public:
    GeomEd();
    void setView(QGraphicsView* c_view);
    void setGridEnabled(bool enabled);
    bool isGridEnabled() const;

    enum Unit { Millimeter, Centimeter, Meter };
    void setUnit(Unit u);
    void setViewScale(double scale);
    double getViewScale() const;


protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    QPointF snapToGrid(QPointF pos);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void checkIntersections(QLineF newLine);
    void splitEdge(QGraphicsLineItem* edge, QPointF p);

private:
    QGraphicsView* view;
    double gridStep = 20.0;
    bool gridEnabled = true;
    QGraphicsEllipseItem* snapMarker;
    QGraphicsLineItem* previewLine;
    QPointF startPoint;
    QGraphicsEllipseItem* startVertex;
    bool drawing;
    QVector<QGraphicsEllipseItem*> vertices;
    QGraphicsEllipseItem* findVertex(QPointF pos);
    QGraphicsEllipseItem* createVertex(QPointF pos);
    QVector<QGraphicsLineItem*> edges;
    Unit currentUnit = Millimeter;
    double unitScale = 1.0;
    double viewScale = 1.0;
};

#endif // GEOMED_H
