#include "open.h"
#include "knobs.h"
#include "node.h"

void Open::setup()
{
    //filename = "E:/projektid/progemine/mitab-1.7.0-win32/54754mld.TAB";
    filename = "";
    number = 1;
}

QString Open::description()
{
    setup();
    return QString("Input/Open Table;Open data table./0");
}

void Open::knobs(KnobCallback* f)
{
    setup();
    FileDialog_knob(f, &filename, "File");
    //Integer_knob(f, &number, "Number:");
}

QString Open::engine()
{
    QString command;
    command = QString("Open Table \"%1\" ").arg(filename);
    command += QString("Select * From %1 Into _%2 ").arg(filename.split("/").last().split(".").first()).arg(getHash());

    return command;
}
