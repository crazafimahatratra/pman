#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>

namespace Ui {
class Calendar;
}

class Calendar : public QCalendarWidget
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = 0);
    ~Calendar();

private:
    Ui::Calendar *ui;
};

#endif // CALENDAR_H
