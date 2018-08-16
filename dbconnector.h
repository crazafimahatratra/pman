#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class DbConnector
{
public:
    DbConnector();
    ~DbConnector();
    void init();
    void execute(QString);
    static QSqlQuery getQuery();
private:
    QSqlDatabase database;
    static DbConnector *instance;
    static DbConnector *getInstance();
};

#endif // DBCONNECTOR_H
