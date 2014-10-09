#ifndef VIEWERNODEGRAPH_H
#define VIEWERNODEGRAPH_H

#include <QGraphicsView>
#include "pirilib.h"


class MainWindow;
class NodeGraph;
class Edge;
class SearchDialog;

class PIRILIBSHARED_EXPORT ViewerNodeGraph : public QGraphicsView
{
    Q_OBJECT
public:
    ViewerNodeGraph(NodeGraph *nodeGraph, MainWindow *parent = 0);
    MainWindow* getParent();
    NodeGraph* getNodeGraph();
    int getMode();
    void setMode(int mode);

    void disableSelected();

    void evaluate();

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    void centerView();

    QPoint _lastPos; /*!< Mouse position at last event. */

private:
    MainWindow* myParent; /*! Parent MainWindow */
    NodeGraph* myNodeGraph; /*! Scene that this viewer shows. Main scene DAG!*/

    Edge* activeEdge; /*! Active edge being dragged */
    QGraphicsLineItem *line; /*!< Graphical representation of line. Used in edge dragging? */
    QGraphicsRectItem *selectRect; /*!< Graphical representation of selection rectangle. */
    int onNode; /*!< Is mouse on node?. */

    //SearchDialog* searchDialog;

};

#endif // VIEWERNODEGRAPH_H
