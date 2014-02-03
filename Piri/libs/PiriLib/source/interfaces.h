#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>

#include "pirilib.h"
#include "knobcallback.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
QT_END_NAMESPACE

// Kõik funktsioonid peavad olema "pure virtual", ehk siis lõpus = 0 !!! Muidu tuleb jama.
class PIRILIBSHARED_EXPORT OpInterfaceMI
{
public:
    virtual ~OpInterfaceMI() {}
    virtual QString engine() = 0;
    virtual void knobs(KnobCallback* f) = 0;
    virtual QString description() = 0;
};

QT_BEGIN_NAMESPACE

#define OpInterfaceMI_iid "Kaldera.Piri.v01.OpInterfaceMI"
Q_DECLARE_INTERFACE(OpInterfaceMI, OpInterfaceMI_iid)

QT_END_NAMESPACE
#endif // INTERFACES_H
