#ifndef MDIPROJECT_H
#define MDIPROJECT_H

#include <QDialog>
#include <QTableWidgetItem>
#include "models/project.h"
#include "models/task.h"
#include "models/note.h"
#include "mainwindow.h"

namespace Ui {
class MdiProject;
}

class MdiProject : public QDialog
{
    Q_OBJECT

public:
    explicit MdiProject(QWidget *parent = 0, int project_id = 0);
    ~MdiProject();
    int projectId();
signals:
    void TaskListModified();

private slots:
    void onProjectRemoved(int);

    void onProjectModified(int);

    void on_actionNew_Task_triggered();

    void on_tableWidget_itemSelectionChanged();

    void on_actionDelete_Task_triggered();

    void on_tableWidget_cellChanged(int row, int column);

    void on_actionEdit_Task_triggered();

    void on_actionMarkAsDone_triggered();

    void on_actionMarkAsPending_triggered();

private:
    Ui::MdiProject *ui;
    Project *project;
    Task *selectedTask();
    void updateToolBar();
    void fillTasks();
};

#endif // MDIPROJECT_H
