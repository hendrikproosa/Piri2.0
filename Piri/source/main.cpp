#include <QApplication>
#include <QtWidgets>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;

    // Stylesheet for ui desing
    QFile File(qApp->applicationDirPath() + "/stylesheet.txt");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    qApp->setStyleSheet(StyleSheet);

    mainWindow.show();
    mainWindow.resize(1200, 800);
    return app.exec();
}
