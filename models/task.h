#ifndef TASK_H
#define TASK_H
#include "basemodel.h"
#include <QList>
#include <QDateTime>

class Task : public BaseModel
{
public:
    Task();
    int id;
    int project_id;
    QString label;
    QDateTime date;
    QString comment;
    QChar status;
    bool canceled;
    int insert();
    void remove();
    void update();
    void markAsDone();
    void markAsPending();
    void cancel();
    void activate();
    bool isDone();
    static QList<Task *> findAll();
    static QList<Task *> findByProject(int project_id, bool hideCanceled, bool hideCompleted);
    static QList<Task *> findPendings();
    static QList<Task *> recordsToList(QList<QSqlRecord>);
    static Task *findById(int id);
private:
    void fromRecord(QSqlRecord);
    QMap<QString, QVariant> toMap();
};
#endif // TASK_H
