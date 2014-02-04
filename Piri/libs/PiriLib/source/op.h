#ifndef OP_H
#define OP_H

#include <QtWidgets>
#include "pirilib.h"

class KnobCallback;
class Node;
class Edge;

class PIRILIBSHARED_EXPORT Op
{
public:
    void evaluate();
    void setCallback(KnobCallback *callback);
    KnobCallback* getCallback() {return myCallback;}
    QWidget* getKnob(QString knobName);

    void disabled();

    int numInputs();
    QList<Edge*> inputs();
    QList<Node*> getInputNodes();
    void setParent(Node* node);

    QString getHash();
    Node* getInput(int order);

protected:
    QString myName;
    QString myDesc;
    Node* myParent;
    KnobCallback* myCallback; /*! Op callback. */
    int inputCount; /*! Number of inputs. To be removed. */
    QList<Edge*> myInputs; /*! List of input edges */
    QList<Node*> myInputNodes; /*! List of input nodes. Will replace myInputs */
};

#endif // OP_H
