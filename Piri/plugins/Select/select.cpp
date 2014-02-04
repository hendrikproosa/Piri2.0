#include "select.h"
#include "node.h"
#include "edge.h"

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
    if (!getInput(0))
        return " ";
    QString command;
    command = "Select * from _" + getInput(0)->getHash() + " into _" + getHash();
    return command;
}
