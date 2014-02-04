#ifndef SELECT_H
#define SELECT_H

#include <QObject>
#include <QtPlugin>

#include "pirilib.h"
#include "interfaces.h"
#include "knobcallback.h"
#include "op.h"

class Open : public QObject, public OpInterfaceMI, public Op
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
    QString filename;
    int number;
};

#endif // SELECT_H
