#include "edge.h"
#include "node.h"
#include "nodegraph.h"

#include <math.h>

#include <QPainter>
#include <QtDebug>
#include <QKeyEvent>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

/*!
 * \brief Edge constructor.
 *
 * Creates new edge between source and destination nodes.
 * \param sourceNode Source node, higher in hierarchy
 * \param destNode Destination node, lower in hierarchy
 * \param eType Edge type
 */
Edge::Edge(Node *sourceNode, Node *destNode, int eType)
{
    arrowSize = EDGE_ARROWSIZE;
    setFlag(ItemIsFocusable);
    setFlag(ItemIsMovable, false);
    setFocus(Qt::OtherFocusReason);
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
    edgeType = eType;
    dragged = 0;
    hovered = 0;
    //destNode->getParent()->getParent()->logMessage("Node constructor");

    if (source)
        source->addEdge(this, 0);
    dest->addEdge(this, 0);
    adjust();
}

/*!
 * \brief Get source node.
 * \return Source node
 * @see source
 */
Node *Edge::sourceNode() const
{
    return source;
}

/*!
 * \brief Get destination node.
 * \return Destination node
 * @see dest
 */
Node *Edge::destNode() const
{
    return dest;
}

/*!
 * \brief Set dragged state.
 * @see dragged
 */
void Edge::setDragged(int drag)
{
    dragged = drag;
}


/*!
 * \brief Adjust edge geometry for redraw.
 *
 * This method is called whenever source or destination nodes are manipulated.
 */
void Edge::adjust()
{
    // If no source nor destination, do nothing
    if (!source && !dest)
        return;

    // If no source, edge is in default state hovering above node. Direction
    // depends on edge type.
    if (!source) {
        if (dest->getClassType() == NODE_TYPE_VIEWER)
            setType(EDGE_TYPE_VIEWER);
        // ss - edge source coordinate in node local space
        // of - offset from edge destination (node center)
        // ds - destination node coordinate in scene space
        QPointF ss, of;
        QPointF ds(mapFromItem(dest, 0, 0));
        switch (edgeType) {
        case EDGE_TYPE_VIEWER:
            of.setX(+30);
            of.setY(-NODE_DRAW_WIDTH);
            break;
        case EDGE_TYPE_DEFAULT:
            of.setX(-30);
            of.setY(-NODE_DRAW_WIDTH);
            break;
        case EDGE_TYPE_BASE:
            of.setX(+30);
            of.setY(-NODE_DRAW_WIDTH);
            break;
        case EDGE_TYPE_MASK:
            of.setX(-NODE_DRAW_WIDTH - 2 * EDGE_ARROWSIZE);
            of.setY(0);
            break;
        default:
            of.setX(-30);
            of.setY(-NODE_DRAW_WIDTH);
            break;
        }
        ss.setX(ds.x() + of.x());
        ss.setY(ds.y() + of.y());
        prepareGeometryChange();
        if (!dragged) sourcePoint = ss;
        destPoint = ds;
    }
    // If no destination (does it happen?) Sits below source node.
    else if (!dest)
    {
        QPointF ss(mapFromItem(source, 0, 0));
        QPointF ds;
        ds.setX(ss.x());
        ds.setY(ss.y() + 30);
        prepareGeometryChange();
        sourcePoint = ss;
        destPoint = ds;
    }
    // Both source and destination node set
    else
    {
        if (dest->getClassType() == NODE_TYPE_VIEWER)
            {setType(EDGE_TYPE_VIEWER);}
        QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
        qreal length = line.length();

        prepareGeometryChange();

        // Check if distance between source and destination node is big enough.
        // Unnecessary because edge is not drawn if source and destination node
        // intersect.
        if (length > qreal(20.)) {
            sourcePoint = line.p1();
            destPoint = line.p2();
        } else {
            sourcePoint = destPoint = line.p1();
        }
    }
    update(boundingRect());
}

/*!
 * \brief Disconnect edge from its nodes
 */
void Edge::disconnect()
{
    source->removeEdge(this);
    source = 0;
    adjust();
}

/*!
 * \brief Set edge type. Not used?
 * \param type Edge type code
 */
void Edge::setType(int type)
{
    edgeType = type;
}

/*!
 * \brief Get edge source coordinates in DAG scene
 * \return Coordinates as QPointF
 */
QPointF Edge::getSourcePoint()
{
    return sourcePoint;
}

/*!
 * \brief Set edge source coordinates in DAG scene
 * \param point Coordinates as QPointF
 */
void Edge::setSourcePoint(QPointF point)
{
    sourcePoint = point;
}

/*!
 * \brief Set source node
 * \param node New source node
 */
void Edge::setSourceNode(Node *node)
{
    source = node;
    if (node)
        node->addEdge(this, EDGE_NOT_MAINEDGE);
    adjust();
}

/*!
 * \brief Reimplemented function. Sets bounding rectangle.
 * \return BRect as QRectF
 */
QRectF Edge::boundingRect() const
{
    if (!source && !dest)
        return QRectF();

    qreal extra = (EDGE_BBOX_PENWIDTH + EDGE_ARROWSIZE) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);

}

/*!
 * \brief Reimplemented function. Sets edge shape.
 * \return
 */
QPainterPath Edge::shape() const
{
    QPainterPath path;
    QPolygon shapePolygon;

    int points[] = {(int)sourcePoint.x(), (int)sourcePoint.y(), (int)destPoint.x(), (int)destPoint.y()};
    shapePolygon.setPoints(2, points);
    path.addPolygon(shapePolygon);

    QPainterPathStroker stroker;
    stroker.setWidth(10);
    stroker.setJoinStyle(Qt::MiterJoin);
    QPainterPath newpath = (stroker.createStroke(path) + path).simplified();

    return newpath;
}


/*!
 * \brief Edge paint engine. Draws edge geometry in DAG.
 * \param painter
 */
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    adjust();
    if (!source && !dest)
        return;

    QPointF p2;
    QPointF intersectPoint, oldDest;
    QLineF polyLine;
    QLineF centerLine;
    QPolygonF endPolygon;
    QPointF p1;
    QLineF line;

    if (source && dest) {

        if (source->collidesWithItem(dest))
            return;

        line.setP1(sourcePoint);
        line.setP2(destPoint);
        if (qFuzzyCompare(line.length(), qreal(0.)))
            return;

        // Joone joonistamine ja lõikepunkti leidmine sõlme kujuga
        centerLine.setPoints(source->pos(), dest->pos());
        QRectF br = dest->boundingRect();
        QPolygonF bp = dest->shape().toFillPolygon();
        br.adjust(0, 0, 0, -5);
        endPolygon = bp;
        p1 = endPolygon.first() + dest->pos();
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + dest->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =  polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }
    }
    else
    {
        QPointF start = sourcePoint;
        line.setP1(start);
        line.setP2(destPoint);

        centerLine.setPoints(start, dest->pos());
        QRectF br = dest->boundingRect();
        QPolygonF bp = dest->shape().toFillPolygon();
        br.adjust(0, 1, 0, -5);
        endPolygon = bp;
        p1 = endPolygon.first() + dest->pos();
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + dest->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }
    }

    QPen viewerPen(Qt::yellow, 2, Qt::DotLine, Qt::SquareCap, Qt::MiterJoin);
    QBrush viewerBrush(Qt::yellow);
    QPen inputPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QBrush inputBrush(Qt::gray);
    QPen selectedPen(QColor::fromRgbF(0.8, 0.6, 0.2, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QBrush selectedBrush(QColor::fromRgbF(0.8, 0.6, 0.2, 1));

    switch (edgeType) {
    case 0:
        painter->setPen(viewerPen);
        painter->setBrush(viewerBrush);
        break;
    case 1:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    case 2:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    case 3:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    case 4:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
        break;
    default:
        painter->setPen(inputPen);
        painter->setBrush(inputBrush);
    }

    line.setP1(sourcePoint);
    if (dest) {
        destPoint = intersectPoint;
    }
    line.setP2(destPoint);

    if (dest->isSelected() && edgeType != 0) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    if (hovered) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    painter->drawLine(line);

    // Kui Control klahv on all ja serval on algus, siis joonistame poolitusmummu
    if (dest->getParent()->getMode() == DAG_MODE_BREAKEDGE && source != 0)
    {
        QPen p = painter->pen();
        painter->setPen(selectedPen);
        painter->drawEllipse((destPoint - sourcePoint)/2 + sourcePoint, NODE_DRAW_RADIUS_DOT, NODE_DRAW_RADIUS_DOT);
        painter->setPen(p);
    }

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;
    oldDest = destPoint;
    destPoint = QPointF(line.x2(), line.y2());

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setPen(inputPen);

    if (edgeType == 0) {
        viewerPen.setStyle(Qt::SolidLine);
        painter->setPen(viewerPen);
    }

    if (dest->isSelected() && edgeType != 0) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    if (hovered) {
        painter->setPen(selectedPen);
        painter->setBrush(selectedBrush);
    }

    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

    painter->setPen(QPen(Qt::white, 0));
    QFont boldFont("Verdana", 8, QFont::Bold);
    QFont normalFont("Verdana", 8, QFont::Normal);
    painter->setFont(normalFont);
    QPointF textPos;
    textPos.setX(intersectPoint.x() + (intersectPoint.x() - oldDest.x())/4 - 10);
    textPos.setY(intersectPoint.y() + (intersectPoint.y() - oldDest.y())/4);

    QString t = "";
    switch (edgeType) {
    case EDGE_TYPE_VIEWER:
        t = "1";
        break;
    case EDGE_TYPE_DEFAULT:
        if (dest->getMaxInputs() > 1)
        {
            t = "A";
        }
        else
        {
            t = "";
        }
        break;
    case EDGE_TYPE_BASE:
        if (dest->getMaxInputs() > 1)
        {
            t = "B";
        }
        else
        {
            t = "";
        }
        break;
    case 3:
        t = "";
        break;
    case 4:
        t = "mask";
        break;
    default:
        break;
    }

    QPen tp = painter->pen();
    painter->setPen(QPen(Qt::black, 0));
    painter->drawText(textPos + QPointF(0.3, 0.3), t);
    painter->setPen(tp);
    painter->drawText(textPos, t);

}
