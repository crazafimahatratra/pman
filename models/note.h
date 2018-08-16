#ifndef NOTE_H
#define NOTE_H

#include "basemodel.h"
#include <QPoint>
#include <QSize>
#include <QDateTime>

class Note : public BaseModel
{
public:
    Note();

    int id;
    int project_id;
    QString title;
    QDateTime ctime;
    QString note;
    QPoint pos;
    QSize size;

    int insert();
    void update();
    void remove();
    QList<Note *> findByProject(int project_id);

private:
    void fromRecord(QSqlRecord *r);
    QMap<QString, QVariant> toMap();
};

#endif // NOTE_H
