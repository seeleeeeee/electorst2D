#include "geometry_scene.h"
#include <QPainter>
#include <cmath>

GeometryScene::GeometryScene(QObject *parent)
    : QGraphicsScene(parent)
    , currentMode(AddEdgeMode)
    , gridEnabled(true)
    , gridSize(20.0)
    , previewEdge(nullptr)
    , startNode(nullptr)
    , hoverNode(nullptr)
    , drawing(false)
    , isDragging(false)
{
    setSceneRect(-500, -500, 1000, 1000);
    qDebug() << "=== SCENE CREATED ===";
}

void GeometryScene::setMode(Mode mode)
{
    currentMode = mode;
    if (mode == AddNodeMode)
        emit statusMessage("Mode: POINTS (click left button)");
    else if (mode == AddEdgeMode)
        emit statusMessage("Mode: LINES (press on point and drag)");
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
    for (auto node : nodes) { removeItem(node); delete node; }
    nodes.clear();
    for (auto edge : edges) { removeItem(edge); delete edge; }
    edges.clear();
    if (previewEdge) { removeItem(previewEdge); delete previewEdge; previewEdge = nullptr; }
    if (hoverNode) { removeItem(hoverNode); delete hoverNode; hoverNode = nullptr; }
    startNode = nullptr;
    drawing = false;
    isDragging = false;
    emit statusMessage("All cleared");
}

QPointF GeometryScene::snapToGridPoint(const QPointF& point)
{
    double x = std::round(point.x() / gridSize) * gridSize;
    double y = std::round(point.y() / gridSize) * gridSize;
    return QPointF(x, y);
}

QGraphicsEllipseItem* GeometryScene::createNode(const QPointF& pos)
{
    QGraphicsEllipseItem* node = addEllipse(-8, -8, 10, 10, QPen(Qt::black, 2), QBrush(Qt::white));
    node->setPos(pos);
    node->setFlag(QGraphicsItem::ItemIsMovable);
    return node;
}

QGraphicsLineItem* GeometryScene::createEdge(const QPointF& p1, const QPointF& p2)
{
    QGraphicsLineItem* edge = addLine(QLineF(p1, p2), QPen(Qt::black, 2));
    edges.append(edge);
    return edge;
}

void GeometryScene::updateHoverNode(const QPointF& pos)
{
    QGraphicsEllipseItem* foundNode = nullptr;
    for (auto node : nodes) {
        if (QLineF(node->pos(), pos).length() < 50) {
            foundNode = node;
            break;
        }
    }
    
    if (foundNode != hoverNode) {
        if (hoverNode) {
            hoverNode->setBrush(QBrush(Qt::white));
            hoverNode->setPen(QPen(Qt::black, 2));
        }
        hoverNode = foundNode;
        if (hoverNode) {
            hoverNode->setBrush(QBrush(Qt::blue));
            hoverNode->setPen(QPen(Qt::darkBlue, 2));
        }
    }
}

void GeometryScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!gridEnabled) return;
    
    painter->setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));
    double left = floor(rect.left() / gridSize) * gridSize;
    double right = ceil(rect.right() / gridSize) * gridSize;
    double top = floor(rect.top() / gridSize) * gridSize;
    double bottom = ceil(rect.bottom() / gridSize) * gridSize;
    
    for (double x = left; x <= right; x += gridSize)
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
    for (double y = top; y <= bottom; y += gridSize)
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    
    painter->setPen(QPen(Qt::red, 2));
    painter->drawLine(QPointF(rect.left(), 0), QPointF(rect.right(), 0));
    painter->drawLine(QPointF(0, rect.top()), QPointF(0, rect.bottom()));
}

void GeometryScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "=== PRESS ===";
    if (event->button() == Qt::LeftButton) {
        QPointF pos = snapToGridPoint(event->scenePos());
        qDebug() << "  pos:" << pos;
        
        if (currentMode == AddNodeMode) {
            QGraphicsEllipseItem* node = createNode(pos);
            nodes.append(node);
            emit statusMessage(QString("Point added: (%1, %2)").arg(pos.x()).arg(pos.y()));
            qDebug() << "  point created, total:" << nodes.size();
        }
        else if (currentMode == AddEdgeMode) {
            startNode = nullptr;
            for (auto node : nodes) {
                double dist = QLineF(node->pos(), pos).length();
                qDebug() << "  checking point" << node->pos() << "distance:" << dist;
                if (dist < 50) {
                    startNode = node;
                    qDebug() << "  >>> FOUND!";
                    break;
                }
            }
            
            if (startNode) {
                isDragging = true;
                drawing = true;
                previewEdge = addLine(QLineF(startNode->pos(), pos), QPen(Qt::gray, 2, Qt::DashLine));
                emit statusMessage("Drag to another point");
                qDebug() << "  line started from:" << startNode->pos();
            } else {
                emit statusMessage("Press on a point! (click on white dot)");
                qDebug() << "  point not found, total points:" << nodes.size();
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void GeometryScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = snapToGridPoint(event->scenePos());
    
    if (currentMode == AddEdgeMode) {
        updateHoverNode(pos);
    }
    
    if (drawing && previewEdge && startNode && isDragging) {
        previewEdge->setLine(QLineF(startNode->pos(), pos));
    }
    
    QGraphicsScene::mouseMoveEvent(event);
}

void GeometryScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "=== RELEASE ===";
    if (event->button() == Qt::LeftButton && drawing && isDragging) {
        QPointF pos = snapToGridPoint(event->scenePos());
        qDebug() << "  released at:" << pos;
        
        QGraphicsEllipseItem* endNode = nullptr;
        for (auto node : nodes) {
            double dist = QLineF(node->pos(), pos).length();
            qDebug() << "  checking point" << node->pos() << "distance:" << dist;
            if (dist < 50 && node != startNode) {
                endNode = node;
                qDebug() << "  >>> END FOUND!";
                break;
            }
        }
        
        if (endNode) {
            createEdge(startNode->pos(), endNode->pos());
            emit statusMessage("LINE ADDED!");
            qDebug() << "  line created!";
        } else {
            emit statusMessage("Release on another point!");
            qDebug() << "  end point not found";
        }
        
        if (previewEdge) {
            removeItem(previewEdge);
            delete previewEdge;
            previewEdge = nullptr;
        }
        
        drawing = false;
        isDragging = false;
        startNode = nullptr;
    }
    
    QGraphicsScene::mouseReleaseEvent(event);
}

QVector<QPointF> GeometryScene::getPolygon() const
{
    QVector<QPointF> polygon;
    if (edges.isEmpty()) return polygon;
    
    QPointF start = edges.first()->line().p1();
    polygon.append(start);
    QPointF current = start;
    
    QVector<bool> used(edges.size(), false);
    
    for (int step = 0; step < edges.size(); step++) {
        bool found = false;
        for (int i = 0; i < edges.size(); i++) {
            if (used[i]) continue;
            QLineF line = edges[i]->line();
            if (line.p1() == current) {
                current = line.p2();
                polygon.append(current);
                used[i] = true;
                found = true;
                break;
            }
            else if (line.p2() == current) {
                current = line.p1();
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
        bool duplicate = false;
        for (const auto& r : result) {
            if (QLineF(p, r).length() < 0.1) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) result.append(p);
    }
    
    return result;
}