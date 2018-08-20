#include "task.h"
#include <QMap>
#include <QVariant>
#include <QDebug>

Task::Task():BaseModel("task", "id")
{
    id = 0;
    status = 'P';
    canceled = false;
}

int Task::insert()
{
    return BaseModel::insert(toMap());
}

void Task::remove()
{
    QMap<QString, QVariant> where;
    where["id = "] = this->id;
    BaseModel::deleteWhere(where);
}

void Task::update()
{
    QMap<QString, QVariant> wheres;   
    wheres["id = "] = this->id;
    BaseModel::update(toMap(), wheres);
}

void Task::markAsDone()
{
    this->status = 'D';
    this->update();
}

void Task::markAsPending()
{
    this->status = 'P';
    this->update();
}

void Task::cancel()
{
    this->canceled = true;
    this->update();
}

void Task::activate()
{
    this->canceled = false;
    this->update();
}

bool Task::isDone()
{
    return this->status == 'D';
}

QList<Task *> Task::findAll()
{
    Task *t = new Task();
    QList<QSqlRecord> res = t->BaseModel::findAll();
    delete t;
    return recordsToList(res);
}

QList<Task *> Task::findByProject(int project_id, bool hideCanceled, bool hideCompleted)
{
    Task *t = new Task();
    QMap<QString, QVariant> wheres;
    wheres["project_id="] = project_id;
    if(hideCanceled)
    {
        wheres["canceled = "] = false;
    }

    if(hideCompleted)
    {
        wheres["status = "] = "P";
    }

    QList<QSqlRecord> res = t->BaseModel::findWhere(wheres);
    delete t;
    return recordsToList(res);
}

QList<Task *> Task::findPendings()
{
    Task *t  = new Task();
    QMap<QString, QVariant> wheres;
    wheres["status="] = "P";
    QList<QSqlRecord> res = t->BaseModel::findWhere(wheres);
    delete t;
    return recordsToList(res);
}

QList<Task *> Task::recordsToList(QList<QSqlRecord> res)
{
    QList<Task *> rows;
    for(int i = 0; i < res.length(); i++)
    {
        Task *t = new Task();
        t->fromRecord(res[i]);
        rows.append(t);
    }
    return rows;
}

Task *Task::findById(int id)
{
    Task *t = new Task();
    QSqlRecord *rec = t->BaseModel::findById(id);
    t->fromRecord(*rec);
    return t;
}

void Task::fromRecord(QSqlRecord res)
{
    this->id = res.value("id").toInt();
    this->project_id = res.value("project_id").toInt();
    this->date = res.value("date").toDateTime();
    this->label = res.value("label").toString();
    this->comment = res.value("comment").toString();
    this->status = res.value("status").toString().at(0);
    this->canceled = res.value("canceled").toBool();
}

QMap<QString, QVariant> Task::toMap()
{
    QMap<QString, QVariant> data;
    data["project_id"] = this->project_id;
    data["label"] = this->label;
    data["date"] = this->date;
    data["comment"] = this->comment;
    data["status"] = this->status;
    data["canceled"] = this->canceled;
    return data;
}
