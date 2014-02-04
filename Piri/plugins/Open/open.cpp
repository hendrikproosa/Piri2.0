#include "open.h"
#include "knobs.h"
#include "node.h"

void Open::setup()
{
    filename = "E:\\projektid\\progemine\\mitab-1.7.0-win32\\54754mld.TAB";
    number = 1;
}

QString Open::description()
{
    setup();
    return QString("Input/Open Table;Open data table./0");
}

void Open::knobs(KnobCallback* f)
{
    FileDialog_knob(f, &filename, "File:");
    Integer_knob(f, &number, "Number:");
}

QString Open::engine()
{
    QString command;
    command = QString("Open Table \"%1\" As _%2 ").arg(filename).arg(getHash());
    //command += "Select * from _54754mld into _" + getHash();

    return command;
}
