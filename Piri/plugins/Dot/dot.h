#ifndef DOT_H
#define DOT_H

#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QImage>
#include "QGraphicsView"
#include <QtCore/qglobal.h>
#include <QStandardItem>

#include "pirilib.h"
#include "interfaces.h"
#include "knobcallback.h"
#include "op.h"

class Dot : public QObject, public OpInterfaceMI, public Op
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Kaldera.Piri.v01.OpInterfaceMI")
    Q_INTERFACES(OpInterfaceMI)

public:
    void setup();
    QString description();
    void knobs(KnobCallback *f);
    QString engine();

protected:

};

#endif // DOT_H
