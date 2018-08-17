#include "dbconnector.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QtWidgets/QMessageBox>

DbConnector* DbConnector::instance = NULL;

DbConnector::DbConnector()
{
    database = QSqlDatabase::addDatabase("QSQLITE", "mydb");
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkdir(path);
    }
    path += "/pman.db";
    database.setDatabaseName(path);
    if(!database.open())
    {
        QMessageBox::critical(0, "Error while creating database", QString("Error while opening database : \n%1").arg(database.lastError().text()));
        exit(EXIT_FAILURE);
    }
    QSqlQuery query(database);
    if(!query.exec("PRAGMA foreign_keys=ON;"))
    {
        qDebug() << query.lastError().text();
    }
    query.clear();
}

DbConnector::~DbConnector()
{
    database.commit();
    database.close();
}

void DbConnector::init()
{
    QFile file(":/sql/model.sql");
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString mytext = in.readAll();
    QStringList sqls = mytext.split(";");
    for(int i = 0; i < sqls.size(); i++)
    {
        if(sqls.at(i).isEmpty())
            continue;
        execute(sqls.at(i));
    }
    file.close();
}

void DbConnector::execute(QString sql)
{
    QSqlQuery query(database);
    if(!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    query.clear();
}

DbConnector *DbConnector::getInstance()
{
    if(instance == NULL)
        instance = new DbConnector();
    return instance;
}

QSqlQuery DbConnector::getQuery()
{
    QSqlQuery query(getInstance()->database);
    return query;
}
