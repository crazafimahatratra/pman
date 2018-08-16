#include "note.h"
#include <QVariant>

Note::Note():BaseModel("note", "id")
{
    title = "New note";
}

int Note::insert()
{
    this->id = BaseModel::insert(toMap());
    return this->id;
}

void Note::update()
{
    QMap<QString, QVariant> wheres;
    wheres["id = "] = this->id;
    BaseModel::update(toMap(), wheres);
}

void Note::remove()
{
    QMap<QString, QVariant> wheres;
    wheres["id = "] = this->id;
    BaseModel::deleteWhere(wheres);
}

QList<Note *> Note::findByProject(int project_id)
{
    QList<Note *> rows;
    QMap<QString, QVariant> wheres;
    wheres["project_id = "] = project_id;
    QList<QSqlRecord> res = BaseModel::findWhere(wheres);
    for(int i = 0; i < res.length(); i++)
    {
        Note *t = new Note();
        t->fromRecord(&res[i]);
        rows.append(t);
    }
    return rows;
}

void Note::fromRecord(QSqlRecord *r)
{
    this->id = r->value("id").toInt();
    this->project_id = r->value("project_id").toInt();
    this->ctime = r->value("ctime").toDateTime();
    this->note = r->value("note").toString();
    this->title = r->value("title").toString();
    int x = r->value("x").toInt();
    int y = r->value("y").toInt();
    this->pos.setX(x);
    this->pos.setY(y);
    this->size.setWidth(r->value("width").toInt());
    this->size.setHeight(r->value("height").toInt());
}

QMap<QString, QVariant> Note::toMap()
{
    QMap<QString, QVariant> data;
    data["project_id"] = this->project_id;
    data["ctime"] = QDateTime::currentDateTime();
    data["note"] = this->note;
    data["title"] = this->title;
    data["x"] = this->pos.x();
    data["y"] = this->pos.y();
    data["width"] = this->size.width();
    data["height"] = this->size.height();
    return data;
}
