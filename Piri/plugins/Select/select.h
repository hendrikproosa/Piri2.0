#ifndef SELECT_H
#define SELECT_H

#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QImage>
#include "QGraphicsView"
#include <QtCore/qglobal.h>
#include <QStandardItem>

#include "pirilib.h"
#include "interfaces.h"

class Select : public QObject, public OpInterfaceMI
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Kaldera.Piri.v01.OpInterfaceMI")
    Q_INTERFACES(OpInterfaceMI)

public:
    void setup();
    QString description();
    void knobs();
    QString engine();

protected:
    int rowFrom;
    int rowTo;
    int colFrom;
    int colTo;

};

#endif // SELECT_H
