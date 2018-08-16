#ifndef DIALOGTASK_H
#define DIALOGTASK_H

#include <QDialog>
#include "models/task.h"

namespace Ui {
class DialogTask;
}

class DialogTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTask(QWidget *parent = 0, int project_id = 0, int task_id = 0);
    ~DialogTask();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DialogTask *ui;
    int project_id;
    Task *task;
};

#endif // DIALOGTASK_H
