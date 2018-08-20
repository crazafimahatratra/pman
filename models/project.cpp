#include "project.h"
#include <QMap>
#include <QVariant>
#include <QPainter>
#include <QDebug>
#include "models/task.h"

Project::Project():BaseModel(TABLE_NAME, "id")
{
    id = 0;
    color = "#000000";
    nb_pendings = 0;
}

int Project::insert()
{
    return BaseModel::insert(toMap());
}

void Project::remove()
{
    QMap<QString, QVariant> where;
    where["id = "] = this->id;
    BaseModel::deleteWhere(where);
}

void Project::update()
{
    QMap<QString, QVariant> wheres;
    wheres["id = "] = this->id;
    BaseModel::update(toMap(), wheres);
}

QMap<QString, QVariant> Project::toMap()
{
    QMap<QString, QVariant> data;
    data["name"] = this->name;
    data["color"] = this->color;
    return data;
}

void Project::fromRecord(QSqlRecord rec)
{
    id = rec.value("id").toInt();
    name = rec.value("name").toString();
    color = rec.value("color").toString();
}

QList<Project *> Project::findAll()
{
    Project *instance = new Project();
    QList<QSqlRecord> res = instance->BaseModel::findAll();
    QList<Project *> rows;
    QMap<int, int> counts = Project::countByProject();
    for(int i = 0; i < res.length(); i++)
    {
        Project *p = new Project();
        p->fromRecord(res[i]);
        if(counts.keys().indexOf(p->id) >= 0)
            p->nb_pendings = counts[p->id];
        rows.append(p);
    }
    delete instance;
    return rows;
}

Project *Project::findById(int id)
{
    Project p0;
    QSqlRecord *record = p0.BaseModel::findById(id);
    if(record != NULL)
    {
        Project *p = new Project();
        Task *t = new Task();
        QMap<QString, QVariant> wheres;
        wheres["project_id = "] = id;
        wheres["status =  "] = "P";
        p->fromRecord(*record);
        p->nb_pendings = t->countWhere(wheres);
        return p;
    }
    return NULL;
}

QIcon Project::toIcon(int size)
{
    QColor bg(this->color);
    QColor fg = bg.darker(120);
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(bg));
    painter.setPen(fg);
    painter.drawRect(1, 1, size - 2, size - 2);
    painter.end();
    return QIcon(pixmap);
}

QIcon Project::toIconBullet(int size)
{
    QColor bg(this->color);
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(bg));
    painter.setPen(Qt::transparent);
    painter.drawEllipse(0, 0, size, size);
    painter.end();
    return QIcon(pixmap);
}

QColor Project::toBgColor()
{
    QColor color(this->color);
    return color.lighter(175);
}

QMap<int, int> Project::countByProject()
{
    Project *p = new Project();
    QString sql = "SELECT count(*) n, project_id FROM task WHERE status = 'P' GROUP BY project_id";
    QList<QSqlRecord> records = p->exec(sql);
    QMap<int, int> map;
    for(int i = 0; i < records.length(); i++)
    {
        map[records[i].value("project_id").toInt()] = records[i].value("n").toInt();
    }
    delete p;
    return map;
}
