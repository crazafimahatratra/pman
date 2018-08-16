#ifndef PROJECT_H
#define PROJECT_H
#include <QString>
#include <QIcon>
#include "basemodel.h"
#include <QList>
#define TABLE_NAME "project"

class Project : public BaseModel
{
public:
    Project();
    int id;
    int nb_pendings;
    QString name;
    QString color;
    int insert();
    QIcon toIcon(int size = 16);
    QIcon toIconBullet(int size = 8);
    QColor toBgColor();
    static QMap<int, int> countByProject();
    static QList<Project *> findAll();
    static Project *findById(int id);
    void remove();
    void update();

private:
    QMap<QString, QVariant> toMap();
    void fromRecord(QSqlRecord rec);
};

#endif // PROJECT_H
