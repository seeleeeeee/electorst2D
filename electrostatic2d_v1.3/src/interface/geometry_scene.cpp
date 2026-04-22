#include "geometry_scene.h"
#include <QPainter>
#include <cmath>

GeometryScene::GeometryScene(QObject *parent)
    : QGraphicsScene(parent)
    , currentMode(AddNodeMode)
    , gridEnabled(true)
    , gridSize(20.0)
    , previewLine(nullptr)
    , startPoint(nullptr)
    , drawing(false)
{
    setSceneRect(-500, -500, 1000, 1000);
    setBackgroundBrush(Qt::white);
}

GeometryScene::~GeometryScene()
{
    clearAll();
}

void GeometryScene::setMode(Mode mode)
{
    currentMode = mode;
    if (mode == AddNodeMode)
        emit statusMessage("Mode: POINTS - click to add point");
    else if (mode == AddEdgeMode)
        emit statusMessage("Mode: LINES - click on point, drag to another point");
    else
        emit statusMessage("Mode: SELECT");
}

void GeometryScene::setGridEnabled(bool enabled)
{
    gridEnabled = enabled;
    update();
}

void GeometryScene::setGridSize(double size)
{
    gridSize = size;
    update();
}

void GeometryScene::clearAll()
{
    drawing = false;
    startPoint = nullptr;
    
    if (previewLine) {
        removeItem(previewLine);
        delete previewLine;
        previewLine = nullptr;
    }
    
    for (auto point : points) {
        removeItem(point);
        delete point;
    }
    points.clear();
    
    for (auto line : lines) {
        removeItem(line);
        delete line;
    }
    lines.clear();
    
    // НЕ вызываем geometryChanged - он отключен
    emit statusMessage("All cleared");
}

QPointF GeometryScene::snapToGrid(const QPointF& pos)
{
    if (!gridEnabled) return pos;
    double x = round(pos.x() / gridSize) * gridSize;
    double y = round(pos.y() / gridSize) * gridSize;
    return QPointF(x, y);
}

void GeometryScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, Qt::white);
    
    if (gridEnabled) {
        painter->setPen(QPen(QColor(200, 200, 200), 1));
        double left = floor(rect.left() / gridSize) * gridSize;
        double right = ceil(rect.right() / gridSize) * gridSize;
        double top = floor(rect.top() / gridSize) * gridSize;
        double bottom = ceil(rect.bottom() / gridSize) * gridSize;
        
        for (double x = left; x <= right; x += gridSize)
            painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
        for (double y = top; y <= bottom; y += gridSize)
            painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    }
    
    painter->setPen(QPen(Qt::red, 2));
    painter->drawLine(QPointF(rect.left(), 0), QPointF(rect.right(), 0));
    painter->drawLine(QPointF(0, rect.top()), QPointF(0, rect.bottom()));
}

QGraphicsEllipseItem* GeometryScene::findPointAt(const QPointF& pos, double tolerance)
{
    for (auto point : points) {
        if (QLineF(point->pos(), pos).length() < tolerance) {
            return point;
        }
    }
    return nullptr;
}

QGraphicsEllipseItem* GeometryScene::createPoint(const QPointF& pos)
{
    QGraphicsEllipseItem* point = addEllipse(-7, -7, 14, 14, QPen(Qt::black, 2), QBrush(Qt::white));
    point->setPos(pos);
    point->setFlag(QGraphicsItem::ItemIsMovable);
    return point;
}

QGraphicsLineItem* GeometryScene::createLine(const QPointF& p1, const QPointF& p2)
{
    QGraphicsLineItem* line = addLine(QLineF(p1, p2), QPen(Qt::black, 2));
    return line;
}

void GeometryScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }
    
    QPointF pos = snapToGrid(event->scenePos());
    
    if (currentMode == AddNodeMode) {
        QGraphicsEllipseItem* point = createPoint(pos);
        points.append(point);
        emit statusMessage(QString("Point at (%1, %2)").arg(pos.x()).arg(pos.y()));
    }
    else if (currentMode == AddEdgeMode) {
        startPoint = findPointAt(pos, 30.0);
        
        if (startPoint) {
            drawing = true;
            previewLine = new QGraphicsLineItem(QLineF(startPoint->pos(), pos));
            previewLine->setPen(QPen(Qt::gray, 2, Qt::DashLine));
            addItem(previewLine);
            emit statusMessage("Drag to another point");
        } else {
            emit statusMessage("Click on a point first!");
        }
    }
}

void GeometryScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = snapToGrid(event->scenePos());
    
    // Подсветка точки под курсором
    for (auto point : points) {
        point->setBrush(QBrush(Qt::white));
        point->setPen(QPen(Qt::black, 2));
    }
    
    QGraphicsEllipseItem* hovered = findPointAt(pos, 30.0);
    if (hovered) {
        hovered->setBrush(QBrush(Qt::blue));
        hovered->setPen(QPen(Qt::darkBlue, 2));
    }
    
    if (drawing && previewLine && startPoint) {
        previewLine->setLine(QLineF(startPoint->pos(), pos));
    }
    
    QGraphicsScene::mouseMoveEvent(event);
}

void GeometryScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        QPointF pos = snapToGrid(event->scenePos());
        
        QGraphicsEllipseItem* endPoint = findPointAt(pos, 30.0);
        
        if (endPoint && endPoint != startPoint) {
            QGraphicsLineItem* line = createLine(startPoint->pos(), endPoint->pos());
            lines.append(line);
            // НЕ вызываем geometryChanged
            emit statusMessage("Line added");
        } else {
            emit statusMessage("Release on another point!");
        }
        
        if (previewLine) {
            removeItem(previewLine);
            delete previewLine;
            previewLine = nullptr;
        }
        
        drawing = false;
        startPoint = nullptr;
    }
    
    QGraphicsScene::mouseReleaseEvent(event);
}

QVector<QPointF> GeometryScene::getPolygon() const
{
    QVector<QPointF> polygon;
    if (lines.isEmpty()) return polygon;
    
    QPointF start = lines.first()->line().p1();
    polygon.append(start);
    QPointF current = start;
    QVector<bool> used(lines.size(), false);
    
    for (int step = 0; step < lines.size(); step++) {
        bool found = false;
        for (int i = 0; i < lines.size(); i++) {
            if (used[i]) continue;
            QLineF l = lines[i]->line();
            if (l.p1() == current) {
                current = l.p2();
                polygon.append(current);
                used[i] = true;
                found = true;
                break;
            }
            else if (l.p2() == current) {
                current = l.p1();
                polygon.append(current);
                used[i] = true;
                found = true;
                break;
            }
        }
        if (!found) break;
    }
    
    QVector<QPointF> result;
    for (const auto& p : polygon) {
        bool dup = false;
        for (const auto& r : result) {
            if (QLineF(p, r).length() < 0.1) { dup = true; break; }
        }
        if (!dup) result.append(p);
    }
    return result;
}

// Заглушки для полигонов
void GeometryScene::clearPolygons() {}
void GeometryScene::setPolygons(const std::vector<electrostatic::Polygon>&) {}
void GeometryScene::addPolygon(const electrostatic::Polygon&) {}
electrostatic::Polygon GeometryScene::createPolygonFromAllEdges(const QString&, const QString&) { 
    return electrostatic::Polygon(); 
}