#ifndef MICONNECT_H
#define MICONNECT_H

#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include "windows.h"

class MIConnect
{
public:
    MIConnect();
    void createConnection(int background);
    void runCommand(QString command);
    QString evalCommand(QString command);
    void browseFromTable(QString tablename);
    void mapFromTable(QString tablename);

    void parentToWindow(QWidget* widget);
    void resizeMapWindow(QWidget* parentWidget);
    void setWindowID(int id);
    int getWindowID();

private:
    QAxWidget* mapInfo; /*! ActiveX object for OLE connection */
    int windowID;
};

#endif // MICONNECT_H
