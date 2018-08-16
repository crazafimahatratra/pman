#ifndef EVENT_H
#define EVENT_H
#include "basemodel.h"
#include <QDateTime>
#include <QVariant>

class Event : public BaseModel
{
public:
    Event();
    int id;
    int project_id;
    QDateTime ctime;
    QString title;
    QString detail;
    int insert();
    void remove();
    void update();
    static QList<Event *> findByDate(QDate date);
    static Event * findById(int id);
    static QList<Event *> findByMonth(int year, int month);

private:
    QMap<QString, QVariant> toMap();
    void fromRecord(QSqlRecord rec);
};

#endif // EVENT_H
