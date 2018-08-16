#include "basemodel.h"
#include <QDebug>
#include "dbconnector.h"

BaseModel::BaseModel(QString table, QString pk)
{
    this->table = table;
    this->pkname = pk;
}

QString BaseModel::whereClause(QMap<QString, QVariant> wheres)
{
    QStringList whereClause;
    for(int i = 0; i < wheres.keys().length(); i++)
    {
        QString k = wheres.keys()[i];
        whereClause.append(k + QString(":params_%1").arg(i));
    }
    return whereClause.join(" AND ");
}

int BaseModel::insert(QMap<QString, QVariant> values)
{
    QStringList columnsInsert;
    QStringList columns;
    for(int i = 0; i < values.keys().length(); i++)
    {
        columnsInsert << ":" + values.keys()[i];
        columns << values.keys()[i];
    }
    QString sql = QString("INSERT INTO %1(%2) VALUES(%3)").arg(table, columns.join(", "), columnsInsert.join(","));
    QSqlQuery query = DbConnector::getQuery();
    PREPARE_QUERY(query, sql, 0)
    for(int i = 0; i < values.keys().length(); i++)
    {
        query.bindValue(":" + values.keys()[i], values.values()[i]);
    }
    EXEC_QUERY(query, , 0)
    return query.lastInsertId().toInt();
}

void BaseModel::deleteWhere(QMap<QString, QVariant> wheres)
{
    QString sql = QString("DELETE FROM %1 WHERE %2").arg(this->table).arg(this->whereClause(wheres));
    QSqlQuery query = DbConnector::getQuery();
    PREPARE_QUERY(query, sql, )
    BIND_WHERE(wheres, query)
    EXEC_QUERY(query, , )
}

void BaseModel::update(QMap<QString, QVariant> sets, QMap<QString, QVariant> wheres)
{
    QStringList values;
    for(int i = 0; i < sets.keys().length(); i++)
    {
        values.append(sets.keys()[i] + QString("=:set_%1").arg(i));
    }
    QString sql = QString("UPDATE %1 SET %2 WHERE %3").arg(table).arg(values.join(", ")).arg(this->whereClause(wheres));
    QSqlQuery query = DbConnector::getQuery();
    PREPARE_QUERY(query, sql, )
    for(int i = 0; i < sets.keys().length(); i++)
    {
        query.bindValue(QString(":set_%1").arg(i), sets.values()[i]);
    }
    BIND_WHERE(wheres, query)
    EXEC_QUERY(query, , )
}

QList<QSqlRecord> BaseModel::findAll()
{
    QString sql = QString("SELECT * FROM %1").arg(this->table);
    QSqlQuery query = DbConnector::getQuery();
    QList<QSqlRecord> records;
    EXEC_QUERY(query, sql, records)
    APPEND_RECORD(query, records)
    return records;
}

QList<QSqlRecord> BaseModel::findWhere(QMap<QString, QVariant> wheres)
{
    QList<QSqlRecord> records;
    QString sql = QString("SELECT * FROM %1 WHERE %2").arg(this->table).arg(whereClause(wheres));
    QSqlQuery query = DbConnector::getQuery();
    PREPARE_QUERY(query, sql, records)
    BIND_WHERE(wheres, query)
    EXEC_QUERY(query, , records)
    APPEND_RECORD(query, records)
    return records;
}

QList<QSqlRecord> BaseModel::exec(QString sql)
{
    QList<QSqlRecord> records;
    QSqlQuery query = DbConnector::getQuery();
    EXEC_QUERY(query, sql, records)
    APPEND_RECORD(query, records)
    return records;
}

QSqlRecord *BaseModel::findById(int id)
{
    QString sql = QString("SELECT * FROM %1 WHERE %2 = %3").arg(this->table).arg(this->pkname).arg(id);
    QSqlQuery query = DbConnector::getQuery();
    EXEC_QUERY(query, sql, NULL)
    if(query.next())
    {
        QSqlRecord *record = new QSqlRecord(query.record());
        return record;
    }
    return NULL;
}

int BaseModel::countWhere(QMap<QString, QVariant> wheres)
{
    QString sql = QString("SELECT COUNT(*) AS N FROM %1 WHERE %2").arg(this->table).arg(whereClause(wheres));
    QSqlQuery query = DbConnector::getQuery();
    PREPARE_QUERY(query, sql, 0)
    BIND_WHERE(wheres, query);
    EXEC_QUERY(query, , 0)
    if(query.next())
    {
        return query.record().value("N").toInt();
    }
    return 0;
}
