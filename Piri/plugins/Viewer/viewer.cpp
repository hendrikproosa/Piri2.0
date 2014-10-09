#include "viewer.h"

void Viewer::setup()
{

}

QString Viewer::description()
{
    setup();
    return QString("Viewer/Viewer;Viewer process./1");
}

void Viewer::knobs(KnobCallback* f)
{

}

QString Viewer::engine()
{
    QString command;
    command = QString("Select * From _%1 Into _%2 ").arg(getInput(0)->getHash()).arg(getHash());
    //command = QString("Browse * From _%1 ").arg(getInput(0)->getHash());

    return command;
}
