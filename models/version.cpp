#include "version.h"
#include "constants.h"
#include <QVariant>
#include <QDebug>
#include <QFile>

Version::Version(): BaseModel("version", "id")
{
}

int Version::currentDbVersion()
{
    QList<QSqlRecord> rec = BaseModel::findAll();
    for(int i = 0; i < rec.count(); i++)
    {
        return rec[i].value("version").toInt();
    }
    return 0;
}

void Version::upgrade()
{
    int dbver = currentDbVersion();
    QFile file(":/sql/patches.sql");
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
        QStringList list = sqls.at(i).split("|");
        if(list.count() < 2)
            continue;
        int v = list.at(0).toInt();
        if(v < dbver) {
            continue;
        }
        qDebug() << v << list.at(1);
        exec(list.at(1));
    }
    file.close();
}
