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
    toolbarTasks->addSeparator();
    toolbarTasks->addAction(ui->actionMarkAsDone);
    toolbarTasks->addAction(ui->actionMarkAsPending);
    toolbarTasks->addSeparator();
    toolbarTasks->addAction(ui->actionCancel_Task);
    toolbarTasks->addAction(ui->actionActivate_Task);

    ui->tableWidget->setColumnWidth(0, 32);
    ui->tableWidget->setColumnWidth(1, 120);

    MainWindow *p = (MainWindow*)this->parentWidget();
    connect(p, SIGNAL(projectRemoved(int)), this, SLOT(onProjectRemoved(int)));
    connect(p, SIGNAL(projectModified(int)), this, SLOT(onProjectModified(int)));
    viewCanceledTasks = p->canceledTasksVisible();
    viewCompletedTasks = p->completedTasksVisible();
    fillTasks();
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

void MdiProject::onViewCanceledTasks(bool checked)
{
    viewCanceledTasks = checked;
    fillTasks();
}

void MdiProject::onViewCompletedTasks(bool checked)
{
    viewCompletedTasks = checked;
    fillTasks();
}

void MdiProject::updateToolBar()
{
    Task *t = selectedTask();
    bool enabled = ui->tableWidget->selectedItems().count() > 0;
    ui->actionEdit_Task->setEnabled(enabled);
    ui->actionDelete_Task->setEnabled(enabled);
    ui->actionMarkAsDone->setEnabled(enabled && !t->isDone());
    ui->actionMarkAsPending->setEnabled(enabled && t->isDone());
    ui->actionCancel_Task->setEnabled(enabled && !t->canceled);
    ui->actionActivate_Task->setEnabled(enabled && t->canceled);
}

void MdiProject::fillTasks()
{
    ui->tableWidget->blockSignals(true);
    QList<Task *> rows = Task::findByProject(this->projectId(), !viewCanceledTasks, !viewCompletedTasks);
    ui->tableWidget->setRowCount(rows.length());
    for(int i = 0; i < rows.length(); i++)
    {
        QTableWidgetItem *itemStatus = new QTableWidgetItem();
        QTableWidgetItem *item = new QTableWidgetItem(rows[i]->date.toString(DATETIME_FORMAT));
        QTableWidgetItem *itemLabel = new QTableWidgetItem(rows[i]->label);
        QString icon = ":/std/icons/clock.png";
        if(rows[i]->status == 'D')
            icon = ":/std/icons/accept.png";
        if(rows[i]->canceled) {
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
            itemStatus->setFont(font);
            itemLabel->setFont(font);
        }
        itemStatus->setData(Qt::UserRole, rows[i]->id);
        itemStatus->setIcon(QIcon(icon));
        itemStatus->setFlags(itemStatus->flags() ^ Qt::ItemIsEditable);
        item->setData(Qt::UserRole, rows[i]->id);
        ui->tableWidget->setItem(i, 0, itemStatus);
        ui->tableWidget->setItem(i, 1, item);
        ui->tableWidget->setItem(i, 2, itemLabel);
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
        emit this->TaskListModified(this->projectId());
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
    emit this->TaskListModified(this->projectId());
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
        emit this->TaskListModified(this->projectId());
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
        emit this->TaskListModified(this->projectId());
    }
}

void MdiProject::on_actionCancel_Task_triggered()
{
    Task *t = selectedTask();
    if(t) {
        t->cancel();
        fillTasks();
        delete t;
    }
}

void MdiProject::on_actionActivate_Task_triggered()
{
    Task *t = selectedTask();
    if(t) {
        t->activate();
        fillTasks();
        delete t;
    }
}
