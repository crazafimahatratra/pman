#ifndef DIALOGMINITASKS_H
#define DIALOGMINITASKS_H

#include <QDialog>
#include <QShowEvent>
#include <QCloseEvent>
#include "models/project.h"

namespace Ui {
class DialogMiniTasks;
}

class DialogMiniTasks : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMiniTasks(QWidget *parent = 0);
    ~DialogMiniTasks();
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
signals:
    void miniWindowClosed();

private:
    Ui::DialogMiniTasks *ui;
    void fillTasks();
    Project *findById(QList<Project *> projects, int id);
};

#endif // DIALOGMINITASKS_H
