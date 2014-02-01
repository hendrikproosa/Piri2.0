#include "viewernodegraph.h"
#include "mainwindow.h"
#include "nodegraph.h"

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
 * \brief Mouse press event subclassed
 * \param event Mouse event
 */
void ViewerNodeGraph::mousePressEvent(QMouseEvent *event)
{

    _lastPos = event->pos();
    if (event->buttons().testFlag(Qt::MiddleButton))
    {
        return;
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

    _lastPos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

/*!
 * \brief Mouse release event subclassed
 * \param event Mouse event
 */
void ViewerNodeGraph::mouseReleaseEvent(QMouseEvent *event)
{
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
