#include "select.h"

#include <QtDebug>
#include <QStandardItemModel>
#include <QTableView>
#include <QList>

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


void Select::knobs()
{

}


QString Select::engine()
{
    QString command;
    command = "Select node command!";

    return command;
}
