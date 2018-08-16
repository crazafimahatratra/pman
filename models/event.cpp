#include "event.h"

Event::Event():BaseModel("event", "id")
{
    id = 0;
}

int Event::insert()
{
    return BaseModel::insert(this->toMap());
}

void Event::remove()
{
    QMap<QString, QVariant> wheres;
    wheres["id = "] = this->id;
    BaseModel::deleteWhere(wheres);
}

void Event::update()
{
    QMap<QString, QVariant> wheres;
    wheres["id = "] = this->id;
    BaseModel::update(toMap(), wheres);
}

QList<Event *> Event::findByDate(QDate date)
{
    Event *instance = new Event();
    QMap<QString, QVariant> wheres;
    wheres["ctime <"] = date.addDays(1);
    wheres["ctime >="] = date;
    QList<QSqlRecord> res = instance->BaseModel::findWhere(wheres);
    QList<Event *> rows;
    for(int i = 0; i < res.length(); i++)
    {
        Event *e = new Event();
        e->fromRecord(res[i]);
        rows.append(e);
    }
    delete instance;
    return rows;
}

Event *Event::findById(int id)
{
    Event *instance = new Event();
    QSqlRecord *rec = instance->BaseModel::findById(id);
    instance->fromRecord(*rec);
    return instance;
}

QList<Event *> Event::findByMonth(int year, int month)
{
    Event *instance = new Event();
    QDate d1(year, month, 1);
    QDate d2 = d1.addMonths(1).addDays(-1);
    QMap<QString, QVariant> wheres;
    wheres["ctime >="] = d1;
    wheres["ctime <="] = d2;
    QList<QSqlRecord> res = instance->BaseModel::findWhere(wheres);
    QList<Event *> rows;
    for(int i = 0; i < res.length(); i++)
    {
        Event *e = new Event();
        e->fromRecord(res[i]);
        rows.append(e);
    }
    delete instance;
    return rows;
}

QMap<QString, QVariant> Event::toMap()
{
    QMap<QString, QVariant> data;
    data["project_id"] = this->project_id;
    data["ctime"] = this->ctime;
    data["title"] = this->title;
    data["detail"] = this->detail;
    return data;
}

void Event::fromRecord(QSqlRecord rec)
{
    this->id = rec.value("id").toInt();
    this->project_id = rec.value("project_id").toInt();
    this->ctime = rec.value("ctime").toDateTime();
    this->title = rec.value("title").toString();
    this->detail = rec.value("detail").toString();
}
