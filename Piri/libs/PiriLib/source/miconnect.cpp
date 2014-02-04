#include "miconnect.h"

/*!
 * \brief MIConnect constructor.
 */
MIConnect::MIConnect()
{
    createConnection(0);
    windowID = 0;
}

/*!
 * \brief Creates connection to MapInfo instance.
 *
 * Parameter background sets if connection is to open session or
 * to new background instance.
 * \param background If set, connects in background
 */
void MIConnect::createConnection(int background)
{
    mapInfo = new QAxWidget;
    mapInfo->setControl("MapInfo.Application");
    QString MId = mapInfo->control();

    if (background)
    {
        mapInfo->setControl(MId);
    } else {
        mapInfo->setControl(MId + "&");
    }
}

/*!
 * \brief Run mapinfo command.
 * \param command Command as string
 */
void MIConnect::runCommand(QString command)
{
    mapInfo->dynamicCall("Do(QString)", command);
}

/*!
 * \brief Run Mapinfo command and return value
 * \param command Command as string
 * \return Return value from mapinfo
 */
QString MIConnect::evalCommand(QString command)
{
    return mapInfo->dynamicCall("Eval(QString)", command).toString();

}

/*!
 * \brief Browse from table tablename
 * \param tablename
 */
void MIConnect::browseFromTable(QString tablename)
{
    runCommand(QString("Browse * From ") + tablename);
}

/*!
 * \brief Map from table tablename
 * \param tablename
 */
void MIConnect::mapFromTable(QString tablename)
{
    runCommand(QString("Map From ") + tablename);
}


/*!
 * \brief Parent next Mapinfo window to widget
 * \param widget Widget to parent to.
 */
void MIConnect::parentToWindow(QWidget* widget)
{
    QString cmd;
    int winHND;
    winHND = (int)widget->winId();
    cmd.append("Set Next Document Parent ");
    cmd.append(QString("%1").arg(winHND));
    cmd.append(" Style 1");
    runCommand(cmd);
}

/*!
 * \brief Resize Mapinfo window that sits on top of parentwidget
 * \param parentWidget Mapinfo window parent
 */
void MIConnect::resizeMapWindow(QWidget* parentWidget)
{
    HWND winHND;
    winHND = (HWND)parentWidget->winId();
    HWND pFoundWindow;
    POINT p;
    p.x = parentWidget->pos().x() + 50;
    p.y = parentWidget->pos().y() + 50;
    pFoundWindow = WindowFromPoint(p);

    if (pFoundWindow != winHND)
        MoveWindow (pFoundWindow, 0, 0, parentWidget->width(), parentWidget->height(), 0);
}

/*!
 * \brief Get mapinfo window id that is shown in Piri
 * \return
 */
int MIConnect::getWindowID()
{
   return windowID;
}

/*!
 * \brief Update mapinfo window id that is shown in piri
 * \param id
 */
void MIConnect::setWindowID(int id)
{
    windowID = id;
}
