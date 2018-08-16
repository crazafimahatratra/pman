#ifndef DIALOGEVENT_H
#define DIALOGEVENT_H

#include <QDialog>
#include <QDate>
#include "models/event.h"

namespace Ui {
class DialogEvent;
}

class DialogEvent : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEvent(QWidget *parent = 0, QDate date = QDate::currentDate(), int event_id = 0);
    ~DialogEvent();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DialogEvent *ui;
    QDate date;
    Event *event;
    void fillProjects();
};

#endif // DIALOGEVENT_H
