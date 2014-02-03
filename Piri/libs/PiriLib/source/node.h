#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

#include "pirilib.h"

class Edge;
class NodeGraph;
class KnobCallback;
//class Knob_Callback;
class OpInterfaceMI;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

int classTypeToInt(QString className);

class PIRILIBSHARED_EXPORT Node : public QGraphicsItem
{
public:
    Node(NodeGraph *nodeGraph, QString name, OpInterfaceMI *op);
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setName(QString name);
    QString getName() { return myName; }
    QString getDesc() { return myDesc; }
    int getClassType() { return classTypeToInt(myClass); }
    NodeGraph* getParent() { return myParent; }
    OpInterfaceMI* getOp() { return myOp; }

    // Methods related to edges
    void addEdge(Edge *edge, int isMain);
    void removeEdge(Edge *edge);
    QList<Edge *> edges() const;
    QList<Edge *> edgesIn() const;
    QList<Edge *> edgesOut() const;
    Edge* getMainEdge() { return mainEdge; }
    void setMaxInputs(int inputs) { maxInputs = inputs; }
    void setNumInputs(int inputs) { numInputs = inputs; }
    int getMaxInputs() { return maxInputs; }
    int getNumInputs() { return numInputs; }

    // Methods related to callback
    void makeCallback();
    KnobCallback* getCallback() { return myCallback; }

    // Op execution method
    void execute();

    void disable(bool val);
    bool isDisabled();

    //QString getHash();


protected:

    void setupInputs();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QString myName; /*!< Node name. First set in Op description. */
    QString myDesc; /*!< Node description. Set in Op description. */
    QString myClass; /*!< Node class. Set in Op description. */
    QList<Edge *> edgeList; /*!< List of all node edges. */
    QPointF newPos; /*!< Some position holder. */
    NodeGraph *myParent; /*!< Nodegraph this node is in. */
    QString nodeHash; /*!< Node hash. Not used. */
    int maxInputs; /*!< Maximum number of inputs. */
    int numInputs; /*!< Number of node inputs. */
    Edge* mainEdge; /*!< Main edge object. */
    bool disabled; /*!< Is node disabled? */

    KnobCallback* myCallback; /*!< Knob callback of node. */

    OpInterfaceMI *myOp; /*!< Node OpInterface. Used instead of myOp in plugins. */
};

#endif // NODE_H
