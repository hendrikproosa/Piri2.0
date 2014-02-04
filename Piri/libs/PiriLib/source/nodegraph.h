#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QStandardItemModel>

#include "pirilib.h"
#include "interfaces.h"

class MainWindow;
class Node;
class Edge;
class MIConnect;

class PIRILIBSHARED_EXPORT NodeGraph : public QGraphicsScene
{
    Q_OBJECT

public:
    NodeGraph(MainWindow *parent = 0);
    MainWindow* getParent();
    int getMode();
    void setMode(int mode);

    QGraphicsItem* pushItem(QGraphicsItem *item, QPointF pos);
    Edge* addEdge(Edge *edge);

    void evaluate();
    QList<Node *> evaluateNode(Node* node);
    void pushNodeEdgesToStack(Node *node);
    QList<Node *> reverseStack(QList<Node *> stack);
    int nodeVisited(Node* node);

    void setSelectedNode();
    Node* getSelectedNode();

    void removeEdge(Edge *edge);
    void removeNode(Node *node);

    // Methods dealing with viewers
    void setActiveViewer(Node* node);
    Node* getActiveViewer();
    void connectViewer(Node* node, int socket);
    void updateViewer();

    // Graph execution method
    void execute();

public slots:
    void addOp(OpInterfaceMI *OpMI);

private:
    MainWindow *myParent; /*!< Nodegraph parent object. */
    int myMode;

    QList<Node *> nodeList; /*!< List of all nodes in nodegraph. */
    QList<Node *> nodeStack; /*!< List of nodes. */
    QList<Node *> evalStack; /*!< List of nodes sorted by execution order. */
    QList<Node *> visitStack; /*!< List of nodes visited. */
    Node* contextSelectedNode;

    Node* activeViewer; /*! Active viewer node, that starts execution */
    MIConnect* miConnect; /*! Mapinfo connection object */
};

#endif // NODEGRAPH_H
