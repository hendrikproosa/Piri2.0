#include "mainwindow.h"
#include "nodegraph.h"
#include "viewernodegraph.h"
#include "interfaces.h"

#include <QtWidgets>
#include <QtDebug>
#include <QScrollArea>

/*!
 * \brief MainWindow initializer.
 *
 * Calls methods to create mainwindow elements.
 * @see createActions()
 * @see createMenus()
 * @see createStatusBar()
 * @see createDockWindows()
 */
MainWindow::MainWindow()
{
    qDebug() << "MainWindow init start...";

    qDebug() << "MainWindow nodegraph...";
    createNodeGraph();

    qDebug() << "MainWindow actions...";
    createActions();

    qDebug() << "MainWindow menus...";
    createMenus();

    qDebug() << "MainWindow statusbar...";
    createStatusBar();

    qDebug() << "MainWindow dockwindows...";
    createDockWindows();

    qDebug() << "MainWindow load plugins...";
    loadPlugins();

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString timeString = date.toString("dd.MM.yyyy") + " " + time.toString();
    setWindowTitle("Piri v.02 - " + timeString);
    qDebug() << "MainWindow initialized!";
}


void MainWindow::showStatusMessage(QString message)
{
    statusBar()->showMessage(message);
}


/*!
 * \brief Creates main application actions.
 *
 * Creates main application actions.
 * @see createMenus()
 * @see close()
 * @see about()
 */
void MainWindow::createActions()
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::HelpContents);
    aboutAct->setStatusTip(tr("Show the About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}


/*!
 * \brief MainWindow about action.
 *
 * Action that gets called when user selects menu item Help->About...
 * @see createActions()
 * @see createMenus()
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Piri"),
             tr("<b>Piri</b> is a node-based GIS application "
                "designed to be fast, intuitive and highly adaptable. <p>"
                "If how it's done is more important than how it looks,"
                "then <b>Piri</b> is made for you!"));
}


/*!
 * \brief MainWindow close action.
 *
 * Action that gets called when user selects menu item File->Close
 * @see createActions()
 * @see createMenus()
 */
void MainWindow::close()
{
    qApp->exit();
}


/*!
 * \brief Creates main menubar and nodegraph popup menu.
 *
 * Creates main UI menubar items and nodegraph popup menu items.
 */
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}


/*!
 * \brief Creates main UI statusbar.
 *
 * Main UI statusbar in the bottom left corner.
 * @see Knob_Callback::showError()
 */
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


/*!
 * \brief Create UI docking windows.
 *
 * Creates main UI docking windows: viewer area, nodegraph, properties view.
 */
void MainWindow::createDockWindows()
{
    QDockWidget* dockViewerArea;

    // Viewer area
    dockViewerArea = new QDockWidget(tr("Viewer"), this);
    dockViewerArea->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dockViewerArea->setMinimumWidth(600);

    /*
    Viewer* viewer = new Viewer(this);
    myViewer = viewer;
    dock->setWidget(viewer);
    */
    QWidget* viewerProxyWidget = new QWidget(this);
    dockViewerArea->setWidget(viewerProxyWidget);

    addDockWidget(Qt::LeftDockWidgetArea, dockViewerArea);

    //Properties view
    QDockWidget* dockPropertiesArea;
    dockPropertiesArea = new QDockWidget(tr("Properties"), this);
    dockPropertiesArea->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dockPropertiesArea->setMinimumWidth(400);
    dockPropertiesArea->setMinimumHeight(200);
    dockPropertiesArea->setMaximumWidth(600);
    QWidget* propertiesProxyWidget = new QWidget();
    //propView = new QWidget;

    QVBoxLayout *propertiesLayout = new QVBoxLayout;
    propertiesLayout->setAlignment(Qt::AlignTop);
    propertiesLayout->setMargin(0);
    propertiesLayout->setSpacing(0);
    propertiesLayout->setSizeConstraint(QLayout::SetMinimumSize);
    propertiesLayout->setDirection(QBoxLayout::BottomToTop);
    propertiesProxyWidget->setLayout(propertiesLayout);

    QScrollArea* propertiesScrollArea = new QScrollArea(this);
    propertiesScrollArea->setWidgetResizable(true);
    propertiesScrollArea->setWidget(propertiesProxyWidget);

    dockPropertiesArea->setWidget(propertiesScrollArea);
    addDockWidget(Qt::RightDockWidgetArea, dockPropertiesArea);

    // Nodegraph view
    QDockWidget* dockNodeGraphArea;
    dockNodeGraphArea = new QDockWidget(tr("Nodegraph"), this);
    dockNodeGraphArea->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dockNodeGraphArea->setMinimumWidth(300);
    dockNodeGraphArea->setMinimumHeight(200);

    ViewerNodeGraph *viewerNodeGraph = new ViewerNodeGraph(nodeGraph, this);
    dockNodeGraphArea->setWidget(viewerNodeGraph);
    addDockWidget(Qt::RightDockWidgetArea, dockNodeGraphArea);

    this->setDockNestingEnabled(true);
}


/*!
 * \brief Create main nodegraph.
 *
 * Creates main nodegraph that holds the DAG.
 */
void MainWindow::createNodeGraph()
{
    nodeGraph = new NodeGraph(this);
}



/*!
 * \brief Loads DLL plugins.
 *
 * Loads all DLL plugins from /plugin directory that are compatible with OpInterface.
 * @see OpInterface
 * @see registerOp()
 * @see populateMenus()
 */
void MainWindow::loadPlugins()
{
    QString pluginFileNames;
    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    qDebug() << "Plugins dir: " << pluginsDir;

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        qDebug() << fileName;
        if (plugin) {
            populateMenus(plugin);
            pluginFileNames += fileName;
        }
    }
    //qDebug() << "Loaded plugins: " << pluginFileNames;
    showStatusMessage("Plugins loaded: " + pluginFileNames);
}


/*!
 * \brief Casts plugins to OpInterfaces and calls registerOp()
 *
 * Casts loaded plugins to OpInterfaces and if successful, sends them to registerOp() for loading.
 * \param plugin Plugin to be cast.
 * @see loadPlugins()
 * @see registerOp()
 * @see OpInterface
 */
void MainWindow::populateMenus(QObject *plugin)
{
    OpInterfaceMI *OpMI = qobject_cast<OpInterfaceMI *>(plugin);
    if (OpMI)
    {
        //registerOp(plugin, OpMI->description(), SLOT(addOp()));
    }
}
