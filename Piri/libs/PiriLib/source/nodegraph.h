#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QStandardItemModel>

#include "pirilib.h"

class MainWindow;

class PIRILIBSHARED_EXPORT NodeGraph : public QGraphicsScene
{
    Q_OBJECT

public:
    NodeGraph(MainWindow *parent = 0);
    MainWindow* getParent();

private:
    MainWindow *myParent; /*!< Nodegraph parent object. */
};

#endif // NODEGRAPH_H
