#include "dialogminitasks.h"
#include "ui_dialogminitasks.h"
#include <QtWidgets/QDesktopWidget>
#include "models/task.h"
#include "models/project.h"
#include <QDebug>

DialogMiniTasks::DialogMiniTasks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMiniTasks)
{
    ui->setupUi(this);
    QRect geom = QApplication::desktop()->screenGeometry();
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);// | Qt::FramelessWindowHint);
    this->move(geom.width() - this->width() - 20, geom.height() - this->height() - 60);
}

DialogMiniTasks::~DialogMiniTasks()
{
    delete ui;
}

void DialogMiniTasks::showEvent(QShowEvent *event)
{
    fillTasks();
    event->accept();
}

void DialogMiniTasks::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit miniWindowClosed();
}

void DialogMiniTasks::fillTasks()
{
    QList<Task *> tasks = Task::findPendings();
    QList<Project *> projects = Project::findAll();
    ui->tableWidget->setRowCount(tasks.length());
    for(int i = 0; i < tasks.length(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(tasks[i]->label);
        Project *p = findById(projects, tasks[i]->project_id);
        if(p) {
            item->setIcon(p->toIcon());
            item->setToolTip(p->name);
        }
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setData(Qt::UserRole, tasks[i]->id);
        ui->tableWidget->setItem(i, 0, item);
    }
}

Project *DialogMiniTasks::findById(QList<Project *> projects, int id)
{
    for(int i = 0; i < projects.length(); i++)
    {
        if(id == projects[i]->id)
            return projects[i];
    }
    return 0;
}
