#ifndef BASEMODEL_H
#define BASEMODEL_H
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QList>

#define PREPARE_QUERY(__Q__, __S__, __R__) if(!__Q__.prepare(__S__)) { qDebug() << __Q__.lastError().text(); return __R__; }
#define EXEC_QUERY(__Q__, __S__, __R__) if(!__Q__.exec(__S__)) { qDebug() << __Q__.lastError().text(); return __R__;}
#define APPEND_RECORD(__Q__, __R__) while((__Q__).next()) { (__R__).append(__Q__.record());}
#define BIND_WHERE(__W__, __Q__) for(int i = 0; i < (__W__).keys().length(); i++) { (__Q__).bindValue(QString(":params_%1").arg(i), (__W__).values()[i]);}

class BaseModel
{
public:
    BaseModel(QString, QString);
    QString whereClause(QMap<QString, QVariant> wheres);

    int insert(QMap<QString, QVariant>);
    void deleteWhere(QMap<QString, QVariant>);
    void update(QMap<QString, QVariant> sets, QMap<QString, QVariant> wheres);
    QList<QSqlRecord> findAll();
    QList<QSqlRecord> findWhere(QMap<QString, QVariant>);
    QList<QSqlRecord> exec(QString sql);
    QSqlRecord *findById(int id);
    int countWhere(QMap<QString, QVariant>);
private:
    QString table;
    QString pkname;
};

#endif // BASEMODEL_H
