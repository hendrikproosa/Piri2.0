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
    contextMenuPos = QPointF(0.0, 0.0);

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

    qDebug() << "Create message log...";
    createMessageLog();

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString timeString = date.toString("dd.MM.yyyy") + " " + time.toString();
    setWindowTitle("Piri v.02 - " + timeString);
    qDebug() << "MainWindow initialized!";
}


/*!
 * \brief Show message on statusbar
 * \param message String to show
 */
void MainWindow::showStatusMessage(QString message)
{
    statusBar()->showMessage(message);
}


/*!
 * \brief Get nodegraph context menu
 * \return Node graph menu QMenu
 */
QMenu* MainWindow::getNodeMenu()
{
    return nodeMenu;
}

/*!
 * \brief Get last nodegraph contextmenu position in DAG scene coords
 * \return Position as QPointF in scene coords
 */
QPointF MainWindow::getContextMenuPos()
{
    return contextMenuPos;
}


/*!
 * \brief Set last nodegraph contextmenu position in DAG scene coords
 * \param pos Position as QPointF
 */
void MainWindow::setContextMenuPos(QPointF pos)
{
    contextMenuPos = pos;
}


/*!
 * \brief Get main properties view.
 * @see createDockWindows()
 * \return Main properties view 'propView'
 */
QWidget* MainWindow::getPropView()
{
    if (propView)
        return propView;
    return 0;
}

/*!
 * \brief Get main properties view layout.
 * @see createDockWindows()
 * @see getPropView()
 * \return Main properties view layout.
 */
QLayout* MainWindow::getPropViewLayout()
{
    if (propView)
    {
        if (propView->layout())
            return propView->layout();
    }
    return 0;
}

/*!
 * \brief Creates main application actions.
 *
 * Creates main application actions.
 * @see createMenus()
 * @see close()
 * @see about()
 * @see showMessageLog()
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

    messageLogAct = new QAction(tr("&Message Log"), this);
    messageLogAct->setStatusTip(tr("Show message log"));
    connect(messageLogAct, SIGNAL(triggered()), this, SLOT(showMessageLog()));
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
 * \brief Add message to messagelog
 * \param message String
 */
void MainWindow::logMessage(QString message)
{
    messageLog.append(message);
    MessageLogText->setText(stringListToString(messageLog));
}

/*!
 * \brief Trigger DAG contextmenu items by name.
 *
 * Useful for calling nodes based on their names.
 * \param name
 */
void MainWindow::triggerMenuByName(QString name)
{
    foreach (QAction *subMenu, nodeMenu->actions())
    {
        foreach (QAction *action, subMenu->menu()->actions())
        {
            if (action->text() == name)
                action->trigger();
        }
    }
}


/*!
 * \brief Converts stringlist to multi-line string
 * \param stringList List to convert
 * \return QString with generated string
 */
QString MainWindow::stringListToString(QStringList stringList)
{
    QString result;
    foreach (QString s, stringList)
    {
        result += s + "\n";
    }
    return result;
}

/*!
 * \brief Creates program message log
 */
void MainWindow::createMessageLog()
{
    messageLogWidget = new QWidget();
    MessageLogText = new QTextEdit(messageLogWidget);
    MessageLogText->setText(stringListToString(messageLog));
    MessageLogText->resize(400, 250);
    messageLogWidget->resize(400, 250);
}

/*!
 * \brief Shows program message log
 */
void MainWindow::showMessageLog()
{
    messageLogWidget->show();
}

/*!
 * \brief Adds new Op to nodegraph.
 * Adds new Op to main nodegraph. At first it casts the sender object to QAction,
 * then casts QAction parent to OpInterfaceMI and calls mainGraph->addOp(Op).
 * @see nodeGraph::addOp()
 */
void MainWindow::addOp()
{
    logMessage("MainWindow::addOp");
    QAction *action = qobject_cast<QAction *>(sender());
    OpInterfaceMI *OpMI = qobject_cast<OpInterfaceMI *>(action->parent());
    nodeGraph->addOp(OpMI);
    messageLog.append("Op Added!");
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
    helpMenu->addAction(messageLogAct);

    // Nodegraph popup menu.
    nodeMenu = new QMenu;

    QStringList menulist;
    menulist << "File"
             << "Edit"
             << "Input"
             << "Output"
             << "Create"
             << "Transform"
             << "Query"
             << "Merge"
             << "MetaData"
             << "Viewer"
             << "Other";

    foreach(QString s, menulist)
    {
        addNodeMenuItem(s);
    }
}


/*!
 * \brief Adds new class to node menu. Not used?
 * \param s Menu item name.
 * @see createMenus()
 */
void MainWindow::addNodeMenuItem(QString menuItemName)
{
    QMenu *menu = new QMenu(menuItemName);
    nodeMenu->addMenu(menu);
    menuClasses.append(menu);
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
    QWidget* propertiesWidget = new QWidget();
    propView = propertiesWidget;

    QVBoxLayout *propertiesLayout = new QVBoxLayout;
    propertiesLayout->setAlignment(Qt::AlignTop);
    propertiesLayout->setMargin(0);
    propertiesLayout->setSpacing(0);
    propertiesLayout->setSizeConstraint(QLayout::SetMinimumSize);
    propertiesLayout->setDirection(QBoxLayout::BottomToTop);
    propertiesWidget->setLayout(propertiesLayout);

    QScrollArea* propertiesScrollArea = new QScrollArea(this);
    propertiesScrollArea->setWidgetResizable(true);
    propertiesScrollArea->setWidget(propertiesWidget);

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

    // Second Nodegraph view
    /*
    QDockWidget* dockNodeGraphArea2;
    dockNodeGraphArea2 = new QDockWidget(tr("Nodegraph2"), this);
    dockNodeGraphArea2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dockNodeGraphArea2->setMinimumWidth(300);
    dockNodeGraphArea2->setMinimumHeight(200);

    ViewerNodeGraph *viewerNodeGraph2 = new ViewerNodeGraph(nodeGraph, this);
    dockNodeGraphArea2->setWidget(viewerNodeGraph2);

    splitDockWidget(dockViewerArea, dockNodeGraphArea2, Qt::Horizontal);
    */

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

    messageLog.append("Loaded plugins:");
    messageLog.append(pluginFileNames);
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
        registerOp(plugin, OpMI->description(), SLOT(addOp()));
    }
}


/*!
 * \brief Registers loaded plugin to system.
 *
 * Splits description of loaded plugin into class, name and description.
 * Creates new QAction and ties it with menu entry and addOp() function
 * for adding new node to graph.
 * Adds new menu class if necessary and new entry for loaded operation.
 * \param plugin Plugin to be registered.
 * \param text Plugin desctiption from Op->description().
 * \param member addOp() function to be tied into signal-slot mechanism.
 * @see loadPlugins()
 * @see populateMenus()
 */
void MainWindow::registerOp(QObject *plugin, const QString text, const char *member)
{
    QString opName, menuName;
    opName = text.split(";").first();
    opName = opName.split("/").last();
    menuName = text.split(";").first();
    menuName = menuName.split("/").first();

    QAction *action = new QAction(opName, plugin);
    connect(action, SIGNAL(triggered()), this, member);

    // Otsime, kas vastav menüü on olemas ja kui pole, siis loome selle
    int exists = 0;
    foreach (QMenu *m, menuClasses)
    {
        if (m->title() == menuName)
            exists = 1;
    }

    if (!exists)
    {
        QMenu *menu = new QMenu(menuName);
        nodeMenu->addMenu(menu);
        menuClasses.append(menu);
    }

    foreach (QMenu *m, menuClasses)
    {
        if (m->title() == menuName)
            m->addAction(action);
    }

    messageLog.append("New plugin:");
    messageLog.append(opName);
    messageLog.append(menuName);
}
