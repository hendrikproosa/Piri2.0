#include "op.h"
#include "edge.h"
#include "node.h"
#include "knobcallback.h"
#include "nodegraph.h"
#include "mainwindow.h"

/*!
 * \brief Calls engine() on Op (through OpInterface)
 * @see OpInterfaceMI::engine()
 */
void Op::evaluate()
{
    myParent->getOp()->engine();
}

/*!
 * \brief Set op callback
 * \param callback
 * @see myCallback
 */
void Op::setCallback(KnobCallback* callback)
{
    myCallback = callback;
}

/*!
 * \brief Set op parent node
 * \param node
 * @see myParent
 */
void Op::setParent(Node *node)
{
    myParent = node;
}

/*!
 * \brief Called instead of evaluate when node is disabled.
 *
 * If node is disabled, calls it's input nodes instead.
 */
void Op::disabled()
{

    if (myParent->edgesIn().count())
    {
        foreach (Edge* e, myParent->edgesIn())
        {
            e->sourceNode()->execute();
        }
    } else {
        // Clear model or do nothing
    }
    /*
    if (myInputNodes.count())
    {
        foreach (Node* n, myInputNodes)
        {
            n->execute();
        }
    }
    */
}

/*!
 * \brief Get number of input nodes. Not used?
 * \return Number of nodes as int
 */
int Op::numInputs()
{
    return inputCount;
}

/*!
 * \brief List of input edges. To be replaced with input nodes instead.
 * \return
 */
QList<Edge*> Op::inputs()
{
    return myInputs;
}

/*!
 * \brief Get list of input nodes
 * \return List of nodes as QList
 */
QList<Node*> Op::getInputNodes()
{
    return myInputNodes;
}


/*!
 * \brief Get knob by name
 * \param knobName Name of knob
 * \return Knob as QWidget
 */
QWidget* Op::getKnob(QString knobName)
{
    return myCallback->getKnob(knobName)->widget;
}

/*!
 * \brief Get hash of op parent node
 * \return Hash as string.
 */
QString Op::getHash()
{
    return myCallback->getParent()->getHash();
}

/*!
 * \brief Get hash of op parent node
 * \return Hash as string.
 */
Node* Op::getInput(int order)
{
    Node* r;
    //r = myCallback->getParent()->edgesIn().first()->sourceNode();
    r = myCallback->getParent()->edgesIn().at(order)->sourceNode();
    if (r)
        return r;
    return 0;
}
