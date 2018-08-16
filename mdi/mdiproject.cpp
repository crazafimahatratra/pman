#include "mdiproject.h"
#include "ui_mdiproject.h"
#include "models/project.h"
#include "models/task.h"
#include "dialogs/dialogtask.h"
#include <QDebug>
#include <QToolBar>
#include <QDateTime>
#include <QtWidgets/QMessageBox>
#include <QMdiSubWindow>

#include "constants.h"
#include "mainwindow.h"

MdiProject::MdiProject(QWidget *parent, int project_id) :
    QDialog(parent),
    ui(new Ui::MdiProject)
{
    ui->setupUi(this);
    if(project_id)
    {
        project = Project::findById(project_id);
        this->setWindowTitle(project->name);
    }
    QToolBar *toolbarTasks = new QToolBar(ui->frame);
    toolbarTasks->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbarTasks->addAction(ui->actionNew_Task);
    toolbarTasks->addAction(ui->actionEdit_Task);
    toolbarTasks->addAction(ui->actionDelete_Task);
    toolbarTasks->addAction(ui->actionMarkAsDone);
    toolbarTasks->addAction(ui->actionMarkAsPending);
    toolbarTasks->addSeparator();

    ui->tableWidget->setColumnWidth(0, 32);
    ui->tableWidget->setColumnWidth(1, 120);
    fillTasks();

    MainWindow *p = (MainWindow*)this->parentWidget();
    connect(p, SIGNAL(projectRemoved(int)), this, SLOT(onProjectRemoved(int)));
    connect(p, SIGNAL(projectModified(int)), this, SLOT(onProjectModified(int)));
}

MdiProject::~MdiProject()
{
    delete ui;
}

int MdiProject::projectId()
{
    if(project != NULL)
        return project->id;
    return 0;
}

void MdiProject::onProjectRemoved(int pid)
{
    if(pid == this->projectId())
    {
        this->parentWidget()->close();
    }
}

void MdiProject::onProjectModified(int pid)
{
    if(pid == this->projectId())
    {
        delete project;
        project = Project::findById(pid);
        this->setWindowTitle(this->project->name);
    }
}

void MdiProject::updateToolBar()
{
    Task *t = selectedTask();
    bool enabled = ui->tableWidget->selectedItems().count() > 0;
    ui->actionEdit_Task->setEnabled(enabled);
    ui->actionDelete_Task->setEnabled(enabled);
    ui->actionMarkAsDone->setEnabled(enabled && !t->isDone());
    ui->actionMarkAsPending->setEnabled(enabled && t->isDone());
}

void MdiProject::fillTasks()
{
    ui->tableWidget->blockSignals(true);
    QList<Task *> rows = Task::findByProject(this->projectId());
    ui->tableWidget->setRowCount(rows.length());
    for(int i = 0; i < rows.length(); i++)
    {
        QTableWidgetItem *itemStatus = new QTableWidgetItem();
        QTableWidgetItem *item = new QTableWidgetItem(rows[i]->date.toString(DATETIME_FORMAT));
        QString icon = ":/std/icons/clock.png";
        if(rows[i]->status == 'D')
            icon = ":/std/icons/accept.png";
        itemStatus->setData(Qt::UserRole, rows[i]->id);
        itemStatus->setIcon(QIcon(icon));
        itemStatus->setFlags(itemStatus->flags() ^ Qt::ItemIsEditable);
        item->setData(Qt::UserRole, rows[i]->id);
        ui->tableWidget->setItem(i, 0, itemStatus);
        ui->tableWidget->setItem(i, 1, item);
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(rows[i]->label));
    }
    ui->tableWidget->blockSignals(false);
    updateToolBar();
}

void MdiProject::on_actionNew_Task_triggered()
{
    DialogTask d(this, this->projectId());
    if(d.exec())
    {
        fillTasks();
        emit this->TaskListModified();
    }
}

void MdiProject::on_tableWidget_itemSelectionChanged()
{
    updateToolBar();
}

void MdiProject::on_actionDelete_Task_triggered()
{
    if(ui->tableWidget->selectedItems().length() == 0)
        return;
    int res = QMessageBox::question(this, "Confirmation", "You sure ?", QMessageBox::Yes|QMessageBox::Cancel);
    if(res != QMessageBox::Yes)
        return;
    Task *t = selectedTask();
    t->remove();
    delete t;
    fillTasks();
    emit this->TaskListModified();
}

void MdiProject::on_tableWidget_cellChanged(int row, int)
{
    Task *t = selectedTask();
    t->date = QDateTime::fromString(ui->tableWidget->item(row, 1)->text(), DATETIME_FORMAT);
    t->label = ui->tableWidget->item(row, 2)->text();
    t->update();
    delete t;
}

void MdiProject::on_actionEdit_Task_triggered()
{
    int row = ui->tableWidget->currentRow();
    int task_id = ui->tableWidget->item(row, 0)->data(Qt::UserRole).toInt();
    DialogTask d(this, this->projectId(), task_id);
    if(d.exec())
        fillTasks();
}

Task *MdiProject::selectedTask()
{
    int row = ui->tableWidget->currentRow();
    if(row < 0)
        return NULL;
    int task_id = ui->tableWidget->item(row, 0)->data(Qt::UserRole).toInt();
    Task *t = Task::findById(task_id);
    return t;
}

void MdiProject::on_actionMarkAsDone_triggered()
{
    Task *t = selectedTask();
    if(t)
    {
        t->markAsDone();
        fillTasks();
        delete t;
        emit this->TaskListModified();
    }
}

void MdiProject::on_actionMarkAsPending_triggered()
{
    Task *t = selectedTask();
    if(t)
    {
        t->markAsPending();
        fillTasks();
        delete t;
        emit this->TaskListModified();
    }
}
