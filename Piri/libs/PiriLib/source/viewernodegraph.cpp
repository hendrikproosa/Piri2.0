#include "viewernodegraph.h"
#include "mainwindow.h"
#include "nodegraph.h"
#include "node.h"
#include "edge.h"

/*!
 * \brief Viewer into nodegraph DAG.
 *
 * There can be multiple views into same DAG.
 * \param nodeGraph Nodegraph that contains DAG
 * \param parent MainWindow that owns viewer
 */
ViewerNodeGraph::ViewerNodeGraph(NodeGraph *nodeGraph, MainWindow *parent)
    : QGraphicsView(parent)
{
    activeEdge = 0;
    myParent = parent;
    myNodeGraph = nodeGraph;
    setScene(nodeGraph);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    scale(qreal(0.8), qreal(0.8));
}

/*!
 * \brief Redirect evaluate calls to main nodegraph.
 */
void ViewerNodeGraph::evaluate()
{
    myNodeGraph->evaluate();
}

/*!
 * \brief Get view parent
 * \return
 */
MainWindow* ViewerNodeGraph::getParent()
{
    return myParent;
}

/*!
 * \brief Get view nodegraph
 * \return
 */
NodeGraph* ViewerNodeGraph::getNodeGraph()
{
    return myNodeGraph;
}

/*!
 * \brief Get mode of main nodegraph.
 * \return Mode as int
 */
int ViewerNodeGraph::getMode()
{
    return myNodeGraph->getMode();
}

/*!
 * \brief Set mode of main nodegraph.
 * \param mode Mode code as int
 */
void ViewerNodeGraph::setMode(int mode)
{
    myNodeGraph->setMode(mode);
}


/*!
 * \brief Disables selected nodes.
 *
 * Disables all selected nodes. Disabled nodes let data pass through unchanged.
 * Sets new state based on which one is in majority: more disabled nodes - enables
 * and vice-versa.
 * @see Node::disable()
 */
void ViewerNodeGraph::disableSelected()
{
    int dis;
    dis = 0;
    foreach (QGraphicsItem *i, scene()->selectedItems()) {
        Node *n = qgraphicsitem_cast<Node *>(i);
        if (n) {
            if (n->isDisabled())
                dis += 1;
        }
    }
    if (2 * dis > scene()->selectedItems().count())
    {
        dis = 0;
    } else {
        dis = 1;
    }
    foreach (QGraphicsItem *i, scene()->selectedItems()) {
        Node *n = qgraphicsitem_cast<Node *>(i);
        if (n) {
            n->disable(dis);
        }
    }
    evaluate();
}


/*!
 * \brief Context menu event.
 *
 * This function shows the nodegraph context menu (right-click menu). It also
 * stores right-click position and node that was selected at event time in
 * variables contextMenuPos and NodeGraph::contextSelectedNode.
 * \param event Event.
 * @see contextMenuPos
 * @see NodeGraph::contextSelectedNode
 */
void ViewerNodeGraph::contextMenuEvent(QContextMenuEvent *event)
{
    myParent->logMessage("ContextMenuEvent");
    myNodeGraph->setSelectedNode();
    QPointF contextMenuPos;
    contextMenuPos = mapToScene(mapFromGlobal(event->globalPos()));
    myParent->setContextMenuPos(contextMenuPos);
    myParent->getNodeMenu()->exec(event->globalPos());
}

/*!
 * \brief Reimplemented keypress event.
 * \param event
 */
void ViewerNodeGraph::keyPressEvent(QKeyEvent *event)
{
    QList<Node *> delList;
    switch (event->key()) {
    case Qt::Key_1:
        if (scene()->selectedItems().count() == 1)
        {
            Node* n = qgraphicsitem_cast<Node*>(scene()->selectedItems().first());
            if (n)
                //connectViewer(n, 1);
                ;
        }
        break;

    case Qt::Key_Control:
        setMode(DAG_MODE_BREAKEDGE);
        break;
    case Qt::Key_D:
        disableSelected();
        break;
    case Qt::Key_Delete:
        /*
        foreach (QGraphicsItem *i, scene()->selectedItems()) {
            if (qgraphicsitem_cast<Node *>(i)) {
                delList << qgraphicsitem_cast<Node *>(i);
            } else {
                scene()->removeItem(i);
                delete i;
            }
        }
        foreach (Node* n, delList) {
            if (n == activeViewer) { activeViewer = 0; }
            removeNode(n);
            delete n;
        }
        evaluate();
        */
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
    QGraphicsView::keyPressEvent(event);
}

/*!
 * \brief Reimplemented key release method.
 *
 * Used to clear up nodegraph mode when mouse actions are interrupted
 * \param event
 */
void ViewerNodeGraph::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        setMode(DAG_MODE_PAN);
        break;
    default:
        ;
    }
    QGraphicsView::keyReleaseEvent(event);
}


/*!
 * \brief Mouse press event subclassed
 * \param event Mouse event
 */
void ViewerNodeGraph::mousePressEvent(QMouseEvent *event)
{
    myParent->logMessage("Mousepressevent");
    _lastPos = event->pos();
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        return;
    }

    activeEdge = 0;
    // Get scene items under mouse cursor
    QList<QGraphicsItem *> startItems =  scene()->items(mapToScene(event->pos()), Qt::IntersectsItemShape);
    // If there are items...
    if (startItems.count() > 0)
    {
        Node *nodeHit = qgraphicsitem_cast<Node *>(startItems.first());
        Edge *edgeHit = qgraphicsitem_cast<Edge *>(startItems.first());
        // If we hit edge set mode to insertedge
        if (edgeHit != 0) {
            if (getMode() != DAG_MODE_BREAKEDGE) {
                setMode(DAG_MODE_INSERTEDGE);
            }
            scene()->clearSelection();
            activeEdge = edgeHit;
            activeEdge->adjust();
            activeEdge->update();
        }
        // If we hit node set mode to PAN
        if (nodeHit != 0) {
            setMode(DAG_MODE_PAN);
        }
    }
    // No items under mouse cursor
    else
    {
        if (event->buttons().testFlag(Qt::LeftButton))
        {
            setMode(DAG_MODE_SELECT);
        }
        else
        {
            setMode(DAG_MODE_PAN);
        }
    }
    // Do different things depending on modes
    Node *node;
    switch (getMode()) {
        case DAG_MODE_INSERTEDGE:
            // If active edge has source, disconnect it.
            // This state is for edge reconnection between nodes
            if (activeEdge->sourceNode()) {
                activeEdge->disconnect();
            }
            line = new QGraphicsLineItem(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
            line->setPen(QPen(QColor::fromRgbF(1.0, 0.0, 0.0, 0.0), 2));
            scene()->addItem(line);
            break;

        // Does nothing. Had some backdrop panning related stuff before
        case DAG_MODE_PAN:
            break;

        // Nothing under mouse, left click, start selecting with rectangle
        case DAG_MODE_SELECT:
            selectRect = new QGraphicsRectItem(QRectF(mapToScene(event->pos()), mapToScene(event->pos())));
            selectRect->setPen(QPen(QColor::fromRgbF(1.0, 0.6, 0.0, 0.7), 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
            selectRect->setBrush(QBrush(QColor::fromRgbF(1.0, 0.6, 0.0, 0.1)));
            scene()->addItem(selectRect);
            break;

        // Create new dot node
        case DAG_MODE_BREAKEDGE:
            myParent->triggerMenuByName("Dot");
            // Move dot to position under mouse.
            // Conveniently it also connects all edges :)
            if (scene()->selectedItems().count() == 1)
                scene()->selectedItems().first()->setPos(mapToScene(event->pos()));
            break;
    default:
        ;
    }

    QGraphicsView::mousePressEvent(event);
}

/*!
 * \brief Mouse move event subclassed
 * \param event Mouse event
 */
void ViewerNodeGraph::mouseMoveEvent(QMouseEvent *event)
{
    QPoint curPos;
    int deltax, deltay;
    curPos = event->pos();
    deltax = _lastPos.x() - curPos.x();
    deltay = _lastPos.y() - curPos.y();
    onNode = 0;

    // Middle button pans scene view
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        QScrollBar* myHScrollbar;
        myHScrollbar = this->horizontalScrollBar();
        myHScrollbar->setValue(myHScrollbar->value() + deltax);

        QScrollBar* myVScrollbar;
        myVScrollbar = this->verticalScrollBar();
        myVScrollbar->setValue(myVScrollbar->value() + deltay);
        _lastPos = curPos;
        return;
    }

    // If we are doing box select...
    // Must see if double testing for selectrect and mode is necessary.
    if (selectRect != 0)
    {
        if (getMode() == DAG_MODE_SELECT)
        {
            QRectF newRect;
            newRect.setTopLeft(mapToScene(curPos));
            newRect.setBottomRight(selectRect->rect().bottomRight());
            selectRect->setRect(newRect);
            scene()->clearSelection();
            QList<QGraphicsItem *> selectedItems = scene()->items(newRect);

            foreach (QGraphicsItem *item, selectedItems) {
                Edge *e = qgraphicsitem_cast<Edge*>(item);
                // For everything but edges, set them selected
                if (e != 0) { }
                else {
                    item->setSelected(true);
                }
            }
            // If we happen to click any other mouse button during selecting
            // release selectrect and remove it.
            if (!event->buttons().testFlag(Qt::LeftButton))
            {
                scene()->removeItem(selectRect);
                selectRect = 0;
                setMode(DAG_MODE_PAN);
            }
        }
        // We are not in selection mode, remove selectrect.
        // This is for removing garbage if something goes wrong.
        else
        {
            scene()->removeItem(selectRect);
            selectRect = 0;
        }
    }

    // We are inserting/dragging an edge
    if (getMode() == DAG_MODE_INSERTEDGE && line != 0)
    {
        QLineF newLine;
        // If we have an active edge that was clicked
        if (activeEdge) {
            newLine.setP1(activeEdge->destNode()->pos());
            newLine.setP2(mapToScene(event->pos()));
            activeEdge->setDragged(1);
            activeEdge->setSourcePoint(mapToScene(event->pos()));
            activeEdge->adjust();
            activeEdge->setVisible(true);
        } else {
            newLine.setPoints(line->line().p1(), mapToScene(event->pos()));
        }
        line->setLine(newLine);

        // Test if there is item under mouse cursor
        QList<QGraphicsItem *> underMouse = scene()->items(mapToScene(curPos));
        if (underMouse.count() > 0)
        {
            Node *um;
            foreach (QGraphicsItem *item, underMouse) {
                um = qgraphicsitem_cast<Node *>(item);
                // If node under cursor is destination node of active edge,
                // do not show the edge.
                if (um == activeEdge->destNode())
                {
                    onNode = 1;
                    activeEdge->setVisible(false);
                }
            }
        }
        // Nothing under mouse cursor, set edge visible
        else
        {
            onNode = 0;
            activeEdge->setVisible(true);
        }

    }

    // Dragging selected nodes around
    if (event->buttons().testFlag(Qt::LeftButton) && getMode() == DAG_MODE_PAN)
    {
        // Walk the edges in scene and set their hovered flag to false
        // This is necessary for node insertion into existing edges. Must not happen here!
        foreach (QGraphicsItem *item, scene()->items())
        {
            Edge *e = qgraphicsitem_cast<Edge*>(item);
            if (e)
            {
                e->hovered = false;
            }
        }
        if (!scene()->selectedItems().isEmpty())
        {
            // Liigutame kõiki valitud objekte
            foreach (QGraphicsItem *item, scene()->selectedItems())
            {
                item->setPos(item->pos() + mapToScene(curPos) - mapToScene(_lastPos));
            }
        }
        // Dragging one node only. This is for node insertion into existing edge.
        if (scene()->selectedItems().count() == 1)
        {
            Node *dn = qgraphicsitem_cast<Node*>(scene()->selectedItems().first());
            if (dn)
            {
                QList<QGraphicsItem *> hoverOverItems = scene()->items(dn->pos());
                foreach (QGraphicsItem *item, hoverOverItems)
                {
                    Edge *de = qgraphicsitem_cast<Edge*>(item);
                    if (de && de->destNode() != dn && de->sourceNode() != dn && dn->getMainEdge())
                    {
                        de->hovered = true;
                    }
                }
            }
        }
    }

    _lastPos = curPos;
    QGraphicsView::mouseMoveEvent(event);
}

/*!
 * \brief Mouse release event subclassed
 * \param event Mouse event
 */
void ViewerNodeGraph::mouseReleaseEvent(QMouseEvent *event)
{
    myParent->logMessage("MouseReleaseEvent");
    // If we are panning around...
    if (getMode() == DAG_MODE_PAN)
    {
        // And have one item selected...
        if (scene()->selectedItems().count() == 1)
        {
            Node *nd;
            nd = qgraphicsitem_cast<Node*>(scene()->selectedItems().first());

            // If hovering over edge, insert node between
            foreach (QGraphicsItem *item, scene()->items())
            {
                Edge *e = qgraphicsitem_cast<Edge*>(item);
                // Must have both edge and node...
                if (e && nd)
                {
                    if (e->hovered && nd->getMainEdge()) {
                        nd->getMainEdge()->setSourceNode(e->sourceNode());
                        e->disconnect();
                        e->setSourceNode(nd);
                        e->hovered = false;
                        e->adjust();
                        //evaluate();
                    }
                }
            }
        }
    }

    // If we are in selection mode remove selection rectangle
    // and set mode to pan.
    if (getMode() == DAG_MODE_SELECT && selectRect != 0)
    {
        this->scene()->removeItem(selectRect);
        selectRect = 0;
        setMode(DAG_MODE_PAN);
    }

    // If we are inserting or dragging an edge
    if (getMode() == DAG_MODE_INSERTEDGE && line != 0)
    {
        QList<QGraphicsItem *> startItems = scene()->items(activeEdge->getSourcePoint());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = scene()->items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        // Remove temporary line item from scene
        scene()->removeItem(line);

        if (activeEdge)
        {
            activeEdge->setDragged(0);
            startItems = endItems;
            endItems.clear();
            endItems.append(activeEdge->destNode());
        }

        // Line does not start from node...
        if (startItems.count() == 0) {
            line = 0;
            activeEdge = 0;
            //evaluate();
            return;
        }

        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first() != endItems.first())
        {
            Node *startNode = qgraphicsitem_cast<Node *>(startItems.first());
            Node *endNode = qgraphicsitem_cast<Node *>(endItems.first());

            // If edge does not have start or end or starts from viewer,
            // we do not create new edge.
            if (!endNode || !startNode || (startNode->getClassType()) == NODE_TYPE_VIEWER)
            {
                line = 0;
                viewport()->setCursor(Qt::ArrowCursor);
                // If dragged edge has source node, disconnect it.
                if (activeEdge->sourceNode()) {
                    activeEdge->disconnect();
                }
                activeEdge->adjust();
                evaluate();
                QGraphicsView::mouseReleaseEvent(event);
                return;
            }

            // Check if there already is edge between start and end nodes
            int edgeExists = 0;
            foreach (Edge *e, endNode->edgesIn())
            {
                if (e->sourceNode() == startNode || startNode == activeEdge->destNode())
                {
                    edgeExists = 1;
                }
            }

            // If endnode is a read node, do not connect
            if (endNode->getClassType() == NODE_TYPE_READ) {
                line = 0;
                viewport()->setCursor(Qt::ArrowCursor);
                QGraphicsView::mouseReleaseEvent(event);
                return;
            }

            // If there is no edge between start and end node...
            if (edgeExists == 0) {
                QList<Node *> evalList;
                evalList = myNodeGraph->evaluateNode(startNode);
                if (!evalList.contains(endNode))
                {
                    if (activeEdge)
                    {
                        if (getMode() != DAG_MODE_BREAKEDGE && activeEdge->sourceNode() != startNode) {
                            activeEdge->setSourceNode(startNode);
                        }
                    }
                    else
                    {
                        Edge *edge = new Edge(startNode, endNode, EDGE_TYPE_BASE);
                        scene()->addItem(edge);
                    }
                }
            }
        }
        evaluate();
    }
    line = 0;
    selectRect = 0;
    activeEdge = 0;

    QGraphicsView::mouseReleaseEvent(event);
}


/*!
 * \brief Mouse wheel event subclassed
 * \param event Mouse event
 */
void ViewerNodeGraph::wheelEvent(QWheelEvent *event)
{
    QPointF s = mapToScene(_lastPos);
    QPointF e, d;

    scaleView(pow((double)2, event->delta() / 500.0));

    e = mapToScene(_lastPos);
    s = mapFromScene(s);
    e = mapFromScene(e);
    d = s - e;

    int deltax, deltay;
    deltax = d.x();
    deltay = d.y();

    QScrollBar* myHScrollbar;
    myHScrollbar = this->horizontalScrollBar();
    myHScrollbar->setValue(myHScrollbar->value() + deltax);

    QScrollBar* myVScrollbar;
    myVScrollbar = this->verticalScrollBar();
    myVScrollbar->setValue(myVScrollbar->value() + deltay);
}


/*!
 * \brief Scales view by scaleFactor.
 *
 * This is a helper function for mouse wheel zoom.
 * \param scaleFactor Factor to scale view by.
 */
void ViewerNodeGraph::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 5)
        return;
    scale(scaleFactor, scaleFactor);

}


/*!
 * \brief Zoom in.
 *
 * Zooming helper function.
 * @see scaleView()
 */
void ViewerNodeGraph::zoomIn()
{
    scaleView(qreal(1.1));
}

/*!
 * \brief Zoom out.
 *
 * Zooming helper function.
 * @see scaleView()
 */
void ViewerNodeGraph::zoomOut()
{
    scaleView(1 / qreal(1.1));
}
