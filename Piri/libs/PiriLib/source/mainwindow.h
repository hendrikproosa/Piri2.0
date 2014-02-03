#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "pirilib.h"

// Forward declarations of classes used
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTableView;
class Viewer;
class QDir;
QT_END_NAMESPACE

class NodeGraph;
class ViewerNodeGraph;

class PIRILIBSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT   // Vajalik signal-slot mehhanismi jaoks
public:
    MainWindow();
    void showStatusMessage(QString message);
    QMenu* getNodeMenu();
    QPointF getContextMenuPos();
    void setContextMenuPos(QPointF pos);

    QWidget* getPropView();
    QLayout* getPropViewLayout();

    void logMessage(QString message);

    void triggerMenuByName(QString name);

private slots:
    void about();
    void close();
    void showMessageLog();
    void addOp();

private:
    void createActions();
    void createMenus();
    void addNodeMenuItem(QString menuItemName);
    void createStatusBar();
    void createDockWindows();
    void createNodeGraph();
    void createMessageLog();
    void loadPlugins();
    void populateMenus(QObject *plugin);
    void registerOp(QObject *plugin, const QString text, const char *member);
    QString stringListToString(QStringList stringList);

    QMenu *fileMenu; /*!< File menu object. */
    QMenu *editMenu; /*!< Edit menu object. */
    QMenu *viewMenu; /*!< View menu object. */
    QMenu *helpMenu; /*!< Help menu object. */

    QMenu *nodeMenu; /*!< Node graph context menu. */
    QList<QMenu*> menuClasses; /*!< List of node graph context menu items. */
    QPointF contextMenuPos;

    QAction *aboutAct; /*!< Shows about dialog. */
    QAction *quitAct; /*!< Closes application. */
    QAction *messageLogAct; /*!< Closes application. */

    QWidget* messageLogWidget;
    QTextEdit* MessageLogText;

    NodeGraph *nodeGraph;

    QWidget *propView; /*!< Dockable properties subwindow. */

    QStringList messageLog;
};

#endif // MAINWINDOW_H
