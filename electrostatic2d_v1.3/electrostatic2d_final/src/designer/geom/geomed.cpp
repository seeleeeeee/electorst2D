#include "geomed.h"
#include <QPainter>
#include <cmath>

GeomEd::GeomEd()
{
    setSceneRect(-5000, -5000, 10000, 10000);

    view = nullptr;
    drawing = false;

    snapMarker = addEllipse(-3, -3, 6, 6, QPen(Qt::blue), QBrush(Qt::blue));
    snapMarker->setZValue(10);
    snapMarker->setFlag(QGraphicsItem::ItemIgnoresTransformations, true); // <- важно!
    previewLine = nullptr;
}

void GeomEd::setView(QGraphicsView *c_view)
{
    view = c_view;
}

void GeomEd::setGridEnabled(bool enabled)
{
    gridEnabled = enabled;
    update();
}
double GeomEd::getViewScale() const{
    return viewScale;
}
bool GeomEd::isGridEnabled() const
{
    return gridEnabled;
}

QPointF GeomEd::snapToGrid(QPointF pos)
{
    if(!gridEnabled) return pos;   // <-- новая проверка
    double step = gridStep * unitScale;
    double x = std::round(pos.x() / step) * step;
    double y = std::round(pos.y() / step) * step;
    return QPointF(x, y);
}

void GeomEd::drawBackground(QPainter *painter, const QRectF &rect)
{
    if(gridEnabled){
        double step = gridStep * unitScale;
        painter->setPen(QPen(QColor(180,180,180)));

        double left = std::floor(rect.left()/step)*step;
        double top  = std::floor(rect.top()/step)*step;

        for(double x = left; x < rect.right(); x += step)
            painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));

        for(double y = top; y < rect.bottom(); y += step)
            painter->drawLine(QLineF(rect.left(), y, rect.right(), y));
    }

    QPen axisPen(Qt::red);
    axisPen.setWidthF(0);
    painter->setPen(axisPen);

    if(rect.left() <= 0 && rect.right() >= 0)
        painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));

    if(rect.top() <= 0 && rect.bottom() >= 0)
        painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));
}

QGraphicsEllipseItem* GeomEd::findVertex(QPointF pos)
{
    const double epsilon = 1e-3;
    for(auto v : vertices)
        if(std::abs(v->pos().x() - pos.x()) < epsilon &&
           std::abs(v->pos().y() - pos.y()) < epsilon)
            return v;
    return nullptr;
}

QGraphicsEllipseItem* GeomEd::createVertex(QPointF pos)
{
    QGraphicsEllipseItem* v = addEllipse(-4,-4,8,8,QPen(Qt::black),QBrush(Qt::black));
    v->setPos(pos);
    vertices.push_back(v);
    return v;
}

void GeomEd::splitEdge(QGraphicsLineItem* edge, QPointF p)
{
    QLineF line = edge->line();
    QPointF p1 = line.p1();
    QPointF p2 = line.p2();

    removeItem(edge);
    edges.removeOne(edge);
    delete edge;

    QGraphicsLineItem* e1 = addLine(QLineF(p1, p), QPen(Qt::black));
    QGraphicsLineItem* e2 = addLine(QLineF(p, p2), QPen(Qt::black));

    edges.push_back(e1);
    edges.push_back(e2);
}

bool intersectSegments(QLineF l1, QLineF l2, QPointF &intersection)
{
    double x1 = l1.p1().x(), y1 = l1.p1().y();
    double x2 = l1.p2().x(), y2 = l1.p2().y();
    double x3 = l2.p1().x(), y3 = l2.p1().y();
    double x4 = l2.p2().x(), y4 = l2.p2().y();

    double denom = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if(std::abs(denom) < 1e-8) return false;

    double px = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4 - y3*x4)) / denom;
    double py = ((x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4 - y3*x4)) / denom;

    if(px < std::min(x1,x2)-1e-6 || px > std::max(x1,x2)+1e-6) return false;
    if(px < std::min(x3,x4)-1e-6 || px > std::max(x3,x4)+1e-6) return false;
    if(py < std::min(y1,y2)-1e-6 || py > std::max(y1,y2)+1e-6) return false;
    if(py < std::min(y3,y4)-1e-6 || py > std::max(y3,y4)+1e-6) return false;

    intersection = QPointF(px, py);
    return true;
}

void GeomEd::checkIntersections(QLineF newLine)
{
    QVector<QGraphicsLineItem*> edgesCopy = edges;
    for(auto edge : edgesCopy)
    {
        QLineF line = edge->line();
        QPointF intersectionPoint;

        if(intersectSegments(line, newLine, intersectionPoint))
        {
            if(!findVertex(intersectionPoint))
                createVertex(intersectionPoint);
            splitEdge(edge, intersectionPoint);
        }
    }
}

void GeomEd::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!drawing) return;

    QPointF snap = snapToGrid(event->scenePos());
    snapMarker->setPos(snap);

    if(previewLine)
        previewLine->setLine(QLineF(startVertex->pos(), snap));

    QGraphicsScene::mouseMoveEvent(event);
}

void GeomEd::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPointF snap = snapToGrid(event->scenePos());
        startVertex = findVertex(snap);
        if(!startVertex) startVertex = createVertex(snap);

        drawing = true;
        previewLine = addLine(QLineF(startVertex->pos(), startVertex->pos()), QPen(Qt::black));
    }

    QGraphicsScene::mousePressEvent(event);
}

void GeomEd::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && drawing)
    {
        QPointF snap = snapToGrid(event->scenePos());
        QGraphicsEllipseItem* endVertex = findVertex(snap);
        if(!endVertex) endVertex = createVertex(snap);

        QLineF newLine(startVertex->pos(), endVertex->pos());
        QGraphicsLineItem* line = addLine(newLine, QPen(Qt::black));
        edges.push_back(line);

        checkIntersections(newLine);

        if(previewLine)
        {
            removeItem(previewLine);
            delete previewLine;
            previewLine = nullptr;
        }

        drawing = false;
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void GeomEd::setUnit(Unit u)
{
    currentUnit = u;
    switch(u)
    {
        case Millimeter: unitScale = 1; break;
        case Centimeter: unitScale = 10; break;
        case Meter: unitScale = 100; break;
    }
    update();
}

void GeomEd::setViewScale(double scale)
{
    if(!view) return;
    viewScale = scale;
    QTransform t;
    t.scale(scale, scale);
    view->setTransform(t);
}
