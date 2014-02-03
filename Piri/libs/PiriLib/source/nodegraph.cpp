#include "nodegraph.h"
#include "mainwindow.h"
#include "node.h"
#include "edge.h"

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
    Node *vNode, *curNode;

    evalStack.clear();
    visitStack.clear();

    vNode = node;
    nodeStack << vNode;
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
 * \brief Evaluate node graph
 */
void NodeGraph::evaluate()
{
    myParent->logMessage("Evaluated graph!");
}
