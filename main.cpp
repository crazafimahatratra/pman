#include "mainwindow.h"
#include <QApplication>
#include "dbconnector.h"
#include "models/version.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Version ver;
    ver.upgrade();
    MainWindow w;
    w.show();

    return a.exec();
}
