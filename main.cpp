#include "mainwindow.h"
#include <QApplication>
#include "dbconnector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    {
        DbConnector db;
        db.init();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
