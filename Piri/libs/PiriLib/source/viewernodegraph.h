#ifndef VIEWERNODEGRAPH_H
#define VIEWERNODEGRAPH_H

#include <QGraphicsView>
#include "pirilib.h"

class MainWindow;
class NodeGraph;

class PIRILIBSHARED_EXPORT ViewerNodeGraph : public QGraphicsView
{
    Q_OBJECT
public:
    ViewerNodeGraph(NodeGraph *nodeGraph, MainWindow *parent = 0);

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void scaleView(qreal scaleFactor);
    void centerView();

    QPoint _lastPos; /*!< Mouse position at last event. */
};

#endif // VIEWERNODEGRAPH_H
