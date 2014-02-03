#include "select.h"

void Select::setup()
{
    rowFrom = 1;
    rowTo = 5;
    colFrom = 1;
    colTo = 5;
}

QString Select::description()
{
    setup();
    return QString("Query/Select;Simple select./1");
}

void Select::knobs(KnobCallback* f)
{

}

QString Select::engine()
{
    QString command;
    command = "Select node command!";

    return command;
}
