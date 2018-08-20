#ifndef PROJECTWIDGETITEM_H
#define PROJECTWIDGETITEM_H

#include <QWidget>

namespace Ui {
class ProjectWidgetItem;
}

class ProjectWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidgetItem(QWidget *parent, QString label, int count);
    ~ProjectWidgetItem();
    void setCount(int);

private:
    Ui::ProjectWidgetItem *ui;
};

#endif // PROJECTWIDGETITEM_H
