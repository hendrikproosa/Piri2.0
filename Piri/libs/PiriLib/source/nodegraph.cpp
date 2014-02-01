#include "nodegraph.h"
#include "mainwindow.h"

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

    QGraphicsItem *text = new QGraphicsTextItem(QString("TERE!!!"));
    text->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    addItem(text);
}
