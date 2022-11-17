#include "MainWindow/mainwindow.h"
#include <QApplication>

int compilerGUI(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
