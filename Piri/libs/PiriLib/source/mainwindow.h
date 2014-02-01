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

private slots:
    void about();
    void close();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void createNodeGraph();
    void loadPlugins();
    void populateMenus(QObject *plugin);

    QMenu *fileMenu; /*!< File menu object. */
    QMenu *editMenu; /*!< Edit menu object. */
    QMenu *viewMenu; /*!< View menu object. */
    QMenu *helpMenu; /*!< Help menu object. */

    QAction *aboutAct; /*!< Shows about dialog. */
    QAction *quitAct; /*!< Closes application. */

    NodeGraph *nodeGraph;


};

#endif // MAINWINDOW_H
