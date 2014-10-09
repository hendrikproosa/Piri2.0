#include "nodegraph.h"
#include "mainwindow.h"
#include "node.h"
#include "edge.h"
#include "miconnect.h"

#include <QtWidgets>


/*!
 * \brief Main nodegraph class that performs operations on nodes.
 *
 * All operations that add, move, connect or delete nodes are performed here.
 * Subclasses QGraphicsScene. There can be multiple views into same nodegraph.
 * \param parent Main UI window
 */
NodeGraph::NodeGraph(MainWindow *parent)
    : QGraphicsScene(parent),
      myParent(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(-10000, -10000, 20000, 20000);
    contextSelectedNode = 0;
    myMode = DAG_MODE_PAN;
    activeViewer = 0;
    //nodeStack = 0;
    //nodeList = 0;
    //evalStack = 0;
    //visitStack = 0;

    miConnect = new MIConnect();
    //addWidget(new QLineEdit("Tere!"));
}


/*!
 * \brief Evaluates node graph starting from node.
 *
 * Almost the same as evaluate() but evaluates node graph starting from
 * specified node.
 * \param node Node to be evaluated.
 * \return Stack of nodes ordered in execution order.
 * @see evaluate()
 * @see pushNodeEdgesToStack()
 * @see nodeVisited()
 */
QList<Node *> NodeGraph::evaluateNode(Node *node)
{
    Node *curNode;

    evalStack << node;
    visitStack << node;

    evalStack.clear();
    visitStack.clear();

    nodeStack << node;
    curNode = node;

    while (nodeStack.length()) {
        evalStack << curNode;
        if (!nodeStack.isEmpty()) {nodeStack.removeLast();}
        pushNodeEdgesToStack(curNode);
        if (nodeStack.isEmpty()) {break;}
        curNode = nodeStack.last();
    }
    foreach (Node *node, reverseStack(evalStack)) {
        if (!nodeVisited(node))
        {
            visitStack << node;
        }
    }
    evalStack = visitStack;
    return evalStack;
}

/*!
 * \brief Add all nodes that are inputs for current node to 'nodeStack'
 *
 * Add all nodes that are connected to current node as inputs to stack of nodes
 * named 'nodeStack'.
 * \param node Node whose inputs are added.
 * @see evaluate()
 * @see evaluateNode()
 * @see Node::edgesIn()
 * @see Edge::sourceNode()
 */
void NodeGraph::pushNodeEdgesToStack(Node *node)
{
    if (!node->edgesIn().isEmpty())
    {
        foreach (Edge* edge, node->edgesIn()) {
            nodeStack << edge->sourceNode();
        }
    }

}

/*!
 * \brief Reverses the order of nodes in stack.
 *
 * Reverses the order of nodes in stack.
 * \param stack Stack to be reversed.
 * \return Reversed node stack.
 */
QList<Node *> NodeGraph::reverseStack(QList<Node *> stack)
{
    QList<Node *> tempStack;
    while (stack.length())
    {
        tempStack << stack.takeLast();
    }
    return tempStack;
}


/*!
 * \brief Creates list of nodes in stack.
 *
 * Not used?
 * \param stack Stack which is debugged.
 * \return QString with node names in stack.
 */
QString NodeGraph::debugStack(QList<Node *> stack)
{
    QString evalList;
    if (stack.length() > 0) {
        foreach (Node *node, stack) {
            evalList += QString(node->getName()) + QString("\n");
        }
    } else {
    }
    return evalList;
}


/*!
 * \brief Checks if node is already visited.
 *
 * Checks node against 'visitStack' to see if it is already visited.
 * \param node Node to be checked.
 * \return 1 if visited, 0 if not.
 */
int NodeGraph::nodeVisited(Node *node)
{
    foreach (Node *vnode, visitStack) {
        if (vnode == node) {return 1;}
    }
    return 0;
}

/*!
 * \brief Get node graph parent Mainwindow
 * \return MainWindow
 */
MainWindow* NodeGraph::getParent()
{
    return myParent;
}


/*!
 * \brief Get current nodegraph mode
 * \return Current nodegraph mode. Code definitions are in pirilib.h header.
 * @see pirilih.h
 */
int NodeGraph::getMode()
{
    return myMode;
}

/*!
 * \brief Set current nodegraph mode
 *
 * Code definitions are in pirilib.h header.
 * @see pirilih.h
 */
void NodeGraph::setMode(int mode)
{
    myMode = mode;
}

/*!
 * \brief Gets node that was selected at contextmenuevent time.
 *
 * This method is used for contextmenuevent. If one node is selected, this
 * function returns selected node and new node will be added after that
 * in nodegraph.
 * \return Selected node held in '_contextSelectedNode'
 * @see _contextSelectedNode
 */
Node* NodeGraph::getSelectedNode()
{
    if (contextSelectedNode)
    {
        return contextSelectedNode;
    } else {
        return 0;
    }
}


void NodeGraph::setSelectedNode()
{
    if (selectedItems().count() == 1)
    {
        Node* node = qgraphicsitem_cast<Node*>(selectedItems().first());
        if (node)
        {
            contextSelectedNode = node;
        }
    } else {
            contextSelectedNode = 0;
        }
}

/*!
 * \brief Add new op to DAG.
 *
 * Adds new op to dag and creates a node as graphical representation.
 * \param OpMI Op to be instanced in DAG.
 */
void NodeGraph::addOp(OpInterfaceMI *OpMI)
{
    //myParent->logMessage("NodeGraph::addOp");
    QString opName;
    opName = OpMI->description().split(";").first();
    opName = opName.split("/").last();

    QGraphicsItem *item = 0;

    item = new Node(this, opName, OpMI);
    myParent->logMessage("NodeGraph::addOp item created");
    if (contextSelectedNode)
    {
        //contextMenuPos += QPoint(0.0, 40.0);
        pushItem(item, myParent->getContextMenuPos() + QPoint(0.0, 40.0));
    } else {
        pushItem(item, myParent->getContextMenuPos());
    }

    clearSelection();
    item->setSelected(true);
}


/*!
 * \brief Pushes item to nodegraph at certain position.
 *
 * Pushes QGraphicsItem 'item' to graph at position 'pos'. If item is node,
 * it will be added to 'nodeList'.
 * \param item QGraphicsItem to be pushed.
 * \param pos Position where item will be.
 * @see nodeList
 */
QGraphicsItem* NodeGraph::pushItem(QGraphicsItem *item, QPointF pos)
{
    //myParent->logMessage("NodeGraph::pushItem");
    addItem(item);
    item->setPos(pos);
    if (qgraphicsitem_cast<Node*>(item))
        nodeList << qgraphicsitem_cast<Node*>(item);
    return item;
}


/*!
 * \brief Adds new edge object to graph scene.
 * \param edge Edge to be added.
 * \return Returns the same edge.
 */
Edge* NodeGraph::addEdge(Edge *edge)
{
    this->addItem(edge);
    return edge;
}



/*!
 * \brief Removes edge from scene and deletes it.
 *
 * Detaches edge from source and destination nodes, removes edge object
 * from DAG scene and deletes the edge object.
 * \param edge Edge to be removed.
 * @see Node::removeEdge()
 */
void NodeGraph::removeEdge(Edge *edge)
{
    edge->sourceNode()->removeEdge(edge);
    edge->destNode()->removeEdge(edge);
    removeItem(edge);
    delete edge;
}

/*!
 * \brief Removes node from nodegraph.
 *
 * Removes node from nodegraph and deletes it. Reconnects all edges to node
 * that is higher in graph hierarchy. Deletes node Ops and callbacks. Op deletion is buggy!!!
 * \param node Node to be removed.
 */
void NodeGraph::removeNode(Node *node)
{
    myParent->logMessage(QString("Deleting: %1").arg(node->getCallback()->getParent()->getName()));

    delete node->getCallback();

    Edge *mE = node->getMainEdge();
    Node *mD = 0;
    if (mE) {
        mD = mE->sourceNode();
        if (mD) {
        }
    }

    foreach (Edge *edge, node->edgesIn())
    {
        edge->disconnect();
        removeItem(edge);
    }

    foreach (Edge *edge, node->edges())
    {
        if (edge->destNode() == node) {
            removeItem(edge);
        }
    }

    foreach (Edge *edge, node->edges())
    {
        if (edge->sourceNode() != 0) {
            edge->disconnect();
            edge->setSourceNode(mD);
        }
    }

    QList<Node *> tempList;
    foreach (Node* n, nodeList)
    {
        if (!tempList.contains(n) && n != node) tempList.append(n);
    }
    nodeList.clear();
    nodeList = tempList;
    removeItem(node);
}

/*!
 * \brief Connects viewer to selected node.
 *
 * Viewer socket 'socket' gets connected to selected node 'node'. Socket part
 * does not work at the moment.
 * \param node Node to be connected
 * \param socket Viewer socket that gets connected (not working)
 */
void NodeGraph::connectViewer(Node *node, int socket)
{
    if (!activeViewer)
    {
        foreach(Node* n, nodeList)
        {
            if (n->getClassType() == NODE_TYPE_VIEWER)
            {
                activeViewer = n;
            }
        }
        if (!activeViewer)
            return;
    }
    Edge *e = activeViewer->getMainEdge();
    if (e->sourceNode())
    {
        e->sourceNode()->removeEdge(e);
    }
    e->setSourceNode(node);
    evaluate();
}

/*!
 * \brief Get active viewer node.
 *
 * This function returns active viewer node that is held in 'activeViewer'.
 * \return Active viewer node.
 * @see setActiveViewer()
 */
Node* NodeGraph::getActiveViewer()
{
    return activeViewer;
}


/*!
 * \brief Sets node as active viewer.
 *
 * This function sets selected node as active viewer. Active viewer is held in 'activeViewer'.
 * \param node Node to be set as viewer
 * @see getActiveViewer()
 * @see activeViewer
 */
void NodeGraph::setActiveViewer(Node *node)
{
    activeViewer = node;
}


/*!
 * \brief Update viewer area
 * Clear viewer area and set it's child to viewer table.
 */
void NodeGraph::updateViewer()
{

    miConnect->runCommand(QString("Close Window %1").arg(miConnect->getWindowID()));
    myParent->logMessage("Parent to window...");
    miConnect->parentToWindow(myParent->getViewer());
    int c = 0;
    foreach (Edge *e, activeViewer->edgesIn())
    {
        if (!e->sourceNode())
            c += 1;
    }
    if (c == activeViewer->edgesIn().count())
    {
        return;
    }
    myParent->logMessage("Browse from... " + QString("_") + activeViewer->getHash());
    miConnect->browseFromTable(QString("_") + activeViewer->getHash());
    miConnect->setWindowID(QString(miConnect->evalCommand("WindowID(0)")).toInt());
    miConnect->runCommand(QString("Close Table selection"));
}


/*!
 * \brief Evaluate node graph
 */
void NodeGraph::evaluate()
{
    myParent->clearCommandList();
    myParent->logMessage("Evaluated graph!");
    myParent->logMessage("Commandlist: ");
    myParent->logMessage(myParent->getCommandList());

    execute();
    myParent->logMessage(myParent->getCommandList());
    foreach(QString command, myParent->getCommandList())
    {
        miConnect->runCommand(command);
    }
    updateViewer();
}


/*!
 * \brief Node graph execution method.
 *
 * Calls the execute() function on last node in evaluation stack 'evalStack'.
 * Usually it is the active viewer node.
 * @see evaluate()
 * @see Node::execute()
 */
void NodeGraph::execute()
{
    myParent->logMessage("Execute!");
    if (!activeViewer)
    {
        myParent->logMessage("No active viewer!");
        foreach(Node* n, nodeList)
        {
            if (n->getClassType() == NODE_TYPE_VIEWER)
            {
                activeViewer = n;
                myParent->logMessage("Active viewer: " + activeViewer->getName());
            }
        }
        if (!activeViewer)
            return;
    }
    if (!activeViewer)
    {
        myParent->logMessage("Why no active viewer!");
        return;
    }
    int c = 0;
    foreach (Edge *e, activeViewer->edgesIn())
    {
        if (!e->sourceNode())
            c += 1;
    }
    if (c == activeViewer->edgesIn().count())
    {
        myParent->logMessage("Viewer has no inputs!");
        return;
    }

    evaluateNode(activeViewer);
    myParent->logMessage("Evaluated active viewer!");
    myParent->logMessage(debugStack(evalStack));
    if (evalStack.count() > 1)
    {
        myParent->logMessage("Evalstack last: " + evalStack.last()->getName());
        evalStack.last()->execute();
    }
}
