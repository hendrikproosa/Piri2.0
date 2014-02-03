#include "node.h"
#include "nodegraph.h"
#include "mainwindow.h"
#include "interfaces.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtDebug>


/*!
 * \brief Node constructor. New variant.
 * \param nodeGraph Parent nodegraph.
 * \param name Node name.
 * \param op Node OpInterfaceMI.
 */
Node::Node(NodeGraph *nodeGraph, QString name, OpInterfaceMI *op)
{
    myParent = nodeGraph;
    //myCallback = 0;
    //mainEdge = 0;
    myName = name;
    myClass = "";
    numInputs = 0;
    disabled = false;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);

    myOp = op;

    //setupInputs2();
    //makeCallback();
}




/*!
 * \brief Overrides bounding rectangle.
 *
 * Reimplemented function.
 * \return Bounding rectangle.
 */
QRectF Node::boundingRect() const
{
    QRectF bRect;
    qreal bx = (myName.length() - 10) * 4;
    /*
    if (classTypeToInt(myClass) != 99) {
        qreal bx = (myName.length() - 10) * 4;


        if (bx > 0) { bx += 40; } else { bx = 40; }

        if (!edgesOut().isEmpty()) {
            bRect.setCoords(-bx, -20, bx, 20);
        }
        else {
            bRect.setCoords(-bx, -20, bx, 28);
        }

        bRect.setCoords(-bx, -20, bx, 28);
    } else {
        bRect.setCoords(-7, -7, 7, 7);
    }
    */
    bRect.setCoords(-bx, -20, bx, 28);
    return bRect;
}

/*!
 * \brief Overrides node shape.
 *
 * Reimplemented function.
 * \return Node shape.
 */
QPainterPath Node::shape() const
{
    QPainterPath path;
    /*
    if (classTypeToInt(myClass) < 21) {
        path.addRect(-40, -20, 80, 40);
    }
    if (classTypeToInt(myClass) > 20 && classTypeToInt(myClass) < 31) {
        path.addRoundedRect(QRectF(-40, -20, 80, 40), 10.0, 10.0);
    }
    if (classTypeToInt(myClass) == 99) {
        path.addEllipse(QPointF(0, 0), 6, 6);
    }
    */
    path.addRect(-40, -20, 80, 40);
    return path;
}

/*
int classTypeToInt(QString className)
{
    int result;
    result = NODE_TYPE_DEFAULT;

    if (className == "Create") result = NODE_TYPE_READ;
    if (className == "Viewer") result = NODE_TYPE_VIEWER;
    if (className == "Dot") result = NODE_TYPE_DOT;
    if (className == "Input") result = NODE_TYPE_READ;
    if (className == "Select") result = NODE_TYPE_DEFAULT;
    if (className == "Output") result = NODE_TYPE_WRITE;
    if (className == "Create") result = NODE_TYPE_READ;

    return result;
}
*/

/*!
 * \brief Node painting.
 *
 * Reimplemented function.
 * \param painter
 * \param option
 */
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QColor bottomColor(QColor::fromRgbF(0.6, 0.4, 0.2, 1));
    QColor color2d(QColor::fromRgbF(0.5, 0.6, 1.0, 1));
    QColor disabledC(QColor::fromRgbF(0.4, 0.4, 0.4, 1.0));
    QPen readPen(Qt::darkRed, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen writePen(Qt::darkYellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen viewerPen(Qt::darkYellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen geo2dPen(color2d.darker(150), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(QColor::fromRgbF(0.4, 0.4, 0.4, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen selectedPen(QColor::fromRgbF(0.8, 0.6, 0.2, 1), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen bottomPen(bottomColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QPen dotPen(QColor::fromRgbF(0.7, 0.7, 0.7, 1), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen fontPen(QColor::fromRgbF(0.0, 0.0, 0.0, 1), 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen disabledPen(disabledC.darker(150), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    QBrush readBrush(QColor::fromRgbF(0.8, 0.3, 0.3, 1), Qt::SolidPattern);
    QBrush writeBrush(Qt::yellow);
    QBrush viewerBrush(Qt::yellow);
    QBrush geo2dBrush(color2d);
    QBrush brush(Qt::darkGray);
    QBrush bottomBrush(bottomColor);
    QBrush disabledBrush(disabledC);


    painter->drawRect(QRectF(-36, -16, 72, 32));


    // Joonistame kolmnurgakese alla kui väljundeid pole
    /*
    if (edgesOut().isEmpty() && nodeType != 0) {
        painter->setPen(bottomPen);
        painter->setBrush(bottomBrush);
        painter->drawPolygon(QPolygonF() << QPointF(-8, 15) << QPointF(8, 15) << QPointF(0, 25));
    }
    */

    //if (myClass == "Create") nodeType = 21;
    //if (myClass == "Input") nodeType = 1;
    //if (myClass == "Output") nodeType = 2;
    //if (myName == "Viewer") nodeType = 0;
    //if (myName == "Dot") nodeType = 99;


    /*
    switch (classTypeToInt(myClass)) {
    case 0:
        painter->setPen(viewerPen);
        painter->setBrush(viewerBrush);
        break;
    case 1:
        painter->setPen(readPen);
        painter->setBrush(readBrush);
        break;
    case 2:
        painter->setPen(writePen);
        painter->setBrush(writeBrush);
        break;
    case 99:
        painter->setPen(dotPen);
        break;
    default:
        painter->setPen(pen);
        painter->setBrush(brush);
    }

    if (classTypeToInt(myClass) > 20 && classTypeToInt(myClass) <= 40) {
        painter->setPen(geo2dPen);
        painter->setBrush(geo2dBrush);
    }

    */
    /*
    if (isSelected())
    {
        painter->setPen(selectedPen);
    }

    if (isDisabled())
    {
        //painter->setPen(disabledPen);
        painter->setBrush(disabledBrush);
    }
    */

    /*
    // Node shape
    if (classTypeToInt(myClass) <= 20) {
        painter->drawRect(QRectF(-36, -16, 72, 32));
    }

    if (classTypeToInt(myClass) > 20 && classTypeToInt(myClass) <= 30) {
        painter->drawRoundedRect(QRectF(-36, -16, 72, 32), 10.0, 10.0);
    }


    if (classTypeToInt(myClass) < 99) {
        painter->setPen(fontPen);
        QFont serifFont("Verdana", 10, QFont::Normal);
        painter->setFont(serifFont);
        painter->drawText(QRectF(-60, -18, 120, 35), Qt::AlignCenter, myName);
    } else {
        painter->setBrush(brush);
        painter->drawEllipse(QPointF(0, 0), 5, 5);
    }
    */

    // If disabled
    /*
    if (isDisabled())
    {
        QPen redCross(QColor::fromRgbF(1.0, 0.0, 0.0, 0.7), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(redCross);
        painter->drawLine(QPointF(-36, -16), QPointF(36, 16));
        painter->drawLine(QPointF(-36, 16), QPointF(36, -16));
    }
    */



}
