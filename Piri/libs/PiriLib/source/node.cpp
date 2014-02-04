#include "node.h"
#include "nodegraph.h"
#include "mainwindow.h"
#include "interfaces.h"
#include "knobcallback.h"
#include "op.h"
#include "edge.h"

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
    //myParent->getParent()->logMessage("Node constructor");
    myCallback = 0;
    mainEdge = 0;
    myName = op->description().split(";").first().split("/").last();
    myClass = op->description().split(";").first().split("/").first();
    myDesc = op->description().split(";").last().split("/").first();
    numInputs = 0;
    maxInputs = op->description().split(";").last().split("/").last().toInt();
    disabled = false;
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(1);

    myOp = op;

    setupInputs();
    makeCallback();
}


/*!
 * \brief Node execution routine.
 *
 * This function calls engine() on Op if node has inputs or if it is top node
 * (node that reads or creates data).
 * @see OpInterface::engine()
 */
void Node::execute()
{
    //if ((edges().count() == edgesOut().count()) || edgesIn().count() > 0)
    //{
        if (!isDisabled())
        {
            //nodeHash = getHash();
            //myOp->engine();
            foreach (Edge* e, edgesIn())
            {
                e->sourceNode()->execute();
            }
            myParent->getParent()->appendCommand(myOp->engine());
        } else {
            Op *op = dynamic_cast<Op*>(myOp);
            op->disabled();
        }
    //}
}

/*!
 * \brief Calculate node hash.
 *
 * Node hash is based on the hash of knob callback and hashes of
 * all nodes above this one. If something changes up in node tree,
 * hash also changes.
 * \return
 */
QString Node::getHash()
{
    QString hashString;
    foreach (Edge* e, edgesIn())
    {
        hashString += e->sourceNode()->getHash();
    }
    hashString += myCallback->getHash();
    hashString = generateHash(hashString);
    return hashString;
}


/*!
 * \brief Node input setup.
 *
 * This function sets up node connections and also connects it to Op.
 */
void Node::setupInputs()
{
    Op *op = dynamic_cast<Op*>(myOp);
    op->setParent(this);

    // Loop through inputs from 0 to maxInputs
    for (int input = 0; input < maxInputs; input++)
    {
        // For first input
        if (input == 0)
        {
            // If node can have more than one input
            if (maxInputs > 1)
            {
                // If there is selected node in DAG, add new node below it and reconnect edges
                if (myParent->getSelectedNode())
                {
                    // Set this node as source for all edges starting from selected node
                    foreach(Edge* edge, myParent->getSelectedNode()->edgesOut())
                    {
                        edge->setSourceNode(this);
                    }
                    // Add new edge between selected node and this node
                    addEdge(myParent->addEdge(new Edge(myParent->getSelectedNode(), this, EDGE_TYPE_BASE)), EDGE_IS_MAINEDGE);
                } else {
                    addEdge(myParent->addEdge(new Edge(0, this, EDGE_TYPE_BASE)), EDGE_IS_MAINEDGE);
                }
            } else {
                // Maximum number of inputs is 1 or 0
                if (myParent->getSelectedNode())
                {
                    foreach(Edge* edge, myParent->getSelectedNode()->edgesOut())
                    {
                        edge->setSourceNode(this);
                    }
                    addEdge(myParent->addEdge(new Edge(myParent->getSelectedNode(), this, EDGE_TYPE_DEFAULT)), EDGE_IS_MAINEDGE);
                } else {
                    addEdge(myParent->addEdge(new Edge(0, this, EDGE_TYPE_DEFAULT)), EDGE_IS_MAINEDGE);
                }
            }
        } else {
            addEdge(myParent->addEdge(new Edge(0, this, EDGE_TYPE_DEFAULT)), EDGE_NOT_MAINEDGE);
        }
    }
}


/*!
 * \brief Creates node callback.
 *
 * If callback exists, adds callback widget to properties view layout.
 */
void Node::makeCallback()
{
    Op *op = dynamic_cast<Op*>(myOp);
    if (!op)
    {
        myParent->getParent()->logMessage("Op cast failed!");
        return;
    }
    if (!myCallback) {
        myCallback = new KnobCallback(this);
        myOp->knobs(myCallback);
        op->setCallback(myCallback);
    }
    if (myName != "Dot")
        myParent->getParent()->getPropViewLayout()->addWidget(myCallback);
}


/*!
 * \brief Add new edge.
 * \param edge Edge to add.
 * \param isMain Is this edge main edge?
 */
void Node::addEdge(Edge *edge, int isMain)
{
    numInputs += 1;
    edgeList << edge;
    if (isMain == EDGE_IS_MAINEDGE) {
        mainEdge = edge;
    }
    edge->adjust();
}

/*!
 * \brief Remove edge from node edgelist.
 * \param edge Edge to be removed.
 * @see edgeList
 */
void Node::removeEdge(Edge *edge)
{
    QList<Edge *> returnList;
    foreach (Edge* e, edgeList)
    {
        if (e != edge && !returnList.contains(e)) {returnList.append(e);}
    }
    edgeList.clear();
    edgeList = returnList;
}


/*!
 * \brief Get list of node edges.
 *
 * To avoid accidental doubling in edgeList it makes sure every edge is
 * returned only once.
 * \return List of node edges.
 * @see edgeList
 */
QList<Edge *> Node::edges() const
{
    QList<Edge *> returnList;
    foreach (Edge* e, edgeList) {
        if (!returnList.contains(e)) {
            returnList << e;
        }
    }
    return returnList;
}

/*!
 * \brief Get list of incoming edges.
 * \return List of edges.
 */
QList<Edge *> Node::edgesIn() const
{
    QList<Edge *> returnList;
    if (mainEdge && mainEdge->sourceNode() != 0)
    {
        returnList << mainEdge;
    }
    foreach (Edge* edge, edgeList) {
        if (edge->destNode() == this && edge->sourceNode() != 0) {
            if (!returnList.contains(edge)) {
                returnList << edge;
            }
        }
    }
    return returnList;
}

/*!
 * \brief Get list of outgoing edges.
 * \return List of edges.
 */
QList<Edge *> Node::edgesOut() const
{
    QList<Edge *> returnList;
    foreach (Edge* edge, edgeList) {
        if (edge->sourceNode() == this && edge->destNode() != 0) {
            returnList << edge;
        }
    }
    return returnList;
}

/*!
 * \brief Set node name.
 * \param name New name
 */
void Node::setName(QString name)
{
    myName = name;
    update();
}

/*!
 * \brief Sets node as disabled.
 *
 * Sets node as disabled. Node state is held in variable 'disabled'.
 * Emits update need on boundingRect.
 * \param val State.
 */
void Node::disable(bool val)
{
    disabled = val;
    emit this->update(boundingRect());
}

/*!
 * \brief Is node disabled?
 * \return True or false.
 */
bool Node::isDisabled()
{
    return disabled;
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
    if (classTypeToInt(myClass) != 99) {
        qreal bx = (myName.length() - 10) * 4;


        if (bx > 0) { bx += 40; } else { bx = 40; }
        /*
        if (!edgesOut().isEmpty()) {
            bRect.setCoords(-bx, -20, bx, 20);
        }
        else {
            bRect.setCoords(-bx, -20, bx, 28);
        }
        */
        bRect.setCoords(-bx, -20, bx, 28);
    } else {
        bRect.setCoords(-7, -7, 7, 7);
    }

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

    if (classTypeToInt(myClass) < 21) {
        path.addRect(-40, -20, 80, 40);
    }
    if (classTypeToInt(myClass) > 20 && classTypeToInt(myClass) < 31) {
        path.addRoundedRect(QRectF(-40, -20, 80, 40), 10.0, 10.0);
    }
    if (classTypeToInt(myClass) == 99) {
        path.addEllipse(QPointF(0, 0), 6, 6);
    }
    return path;
}


/*!
 * \brief Converts class name to type code
 *
 * See also defined class types in pirilib.h
 * \param className Class name as string
 * \return Class type code
 */
int classTypeToInt(QString className)
{
    int result;
    result = NODE_TYPE_DEFAULT;

    if (className == "Create") result = NODE_TYPE_READ;
    if (className == "Viewer") result = NODE_TYPE_VIEWER;
    if (className == "Dot") result = NODE_TYPE_DOT;
    if (className == "Other") result = NODE_TYPE_DOT;
    if (className == "Input") result = NODE_TYPE_READ;
    if (className == "Select") result = NODE_TYPE_DEFAULT;
    if (className == "Output") result = NODE_TYPE_WRITE;
    if (className == "Create") result = NODE_TYPE_READ;

    return result;
}


/*!
 * \brief Node painting operation.
 *
 * Reimplemented function, draws node shape in DAG scene.
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
    QBrush selectedBrush(QColor::fromRgbF(0.8, 0.6, 0.2, 1), Qt::SolidPattern);


    painter->drawRect(QRectF(-36, -16, 72, 32));


    // Joonistame kolmnurgakese alla kui väljundeid pole

    /*
    if (edgesOut().isEmpty() && classTypeToInt(myClass) != 0) {
        painter->setPen(bottomPen);
        painter->setBrush(bottomBrush);
        painter->drawPolygon(QPolygonF() << QPointF(-8, 15) << QPointF(8, 15) << QPointF(0, 25));
    }
    */


    switch (classTypeToInt(myClass)) {
    case 0:
        painter->setPen(viewerPen);
        painter->setBrush(viewerBrush);
        break;
    case 1:
        painter->setPen(pen);
        painter->setBrush(brush);
        break;
    case 2:
        painter->setPen(readPen);
        painter->setBrush(readBrush);
        break;
    case 3:
        painter->setPen(writePen);
        painter->setBrush(writeBrush);
        break;
    case 99:
        painter->setPen(dotPen);
        painter->setBrush(brush);
        break;
    default:
        painter->setPen(pen);
        painter->setBrush(brush);
    }

    if (classTypeToInt(myClass) > 20 && classTypeToInt(myClass) <= 40) {
        painter->setPen(geo2dPen);
        painter->setBrush(geo2dBrush);
    }


    if (isSelected())
    {
        painter->setBrush(selectedBrush);
    }

    if (isDisabled())
    {
        //painter->setBrush(disabledBrush);
    }


    // Node shape
    if (classTypeToInt(myClass) <= 20) {
        painter->drawRect(QRectF(-36, -16, 72, 32));
    }

    if (classTypeToInt(myClass) > 20 && classTypeToInt(myClass) <= 30) {
        painter->drawRoundedRect(QRectF(-36, -16, 72, 32), 10.0, 10.0);
    }

    // Node text and stuff
    if (classTypeToInt(myClass) < 99) {
        painter->setPen(fontPen);
        QFont serifFont("Verdana", NODE_DRAW_TEXTSIZE, QFont::Normal);
        painter->setFont(serifFont);
        painter->drawText(QRectF(-60, -18, 120, 35), Qt::AlignCenter, myName);
    } else {
        painter->setPen(dotPen);
        painter->drawEllipse(QPointF(0, 0), 5, 5);
    }


    // If disabled
    if (isDisabled())
    {
        QPen redCross(QColor::fromRgbF(1.0, 0.0, 0.0, 0.7), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(redCross);
        painter->drawLine(QPointF(-36, -16), QPointF(36, 16));
        painter->drawLine(QPointF(-36, 16), QPointF(36, -16));
    }

}


/*!
 * \brief Double click event on node.
 *
 * Reimplemented function. If knob callback is not visible it will be
 * show in properties window.
 * \param event
 */
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    makeCallback();
    if (myCallback)
    {
        if (myCallback->isHidden() && myName != "Dot")
            myCallback->show();
    }
}



/*!
 * \brief Item changed event.
 *
 * Reimplemented function. Tests if node is selected or moved and
 * adjusts edges or colors knob callback differently.
 * \param change
 * \param value
 * \return
 */
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        break;
    case ItemSelectedHasChanged:
        {
        Op* op = dynamic_cast<Op*>(myOp);
        if (op->getCallback())
        {
            if (this->isSelected()) {
                op->getCallback()->setStyleSheet("KnobCallback { border: 2px solid rgb(240, 200, 100) }");
            } else {
                op->getCallback()->setStyleSheet("KnobCallback { border: 0px solid black }");
            }
        }
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
