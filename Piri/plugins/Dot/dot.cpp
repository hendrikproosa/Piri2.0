#include "dot.h"

void Dot::setup()
{

}

QString Dot::description()
{
    setup();
    return QString("Other/Dot;Graph redirection dot./1");
}

void Dot::knobs(KnobCallback* f)
{

}

QString Dot::engine()
{
    QString command;
    command = "";
    return command;
}
