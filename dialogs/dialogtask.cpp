#include "dialogtask.h"
#include "ui_dialogtask.h"
#include "models/task.h"
#include <QDateTime>

DialogTask::DialogTask(QWidget *parent, int project_id, int task_id) :
    QDialog(parent),
    ui(new Ui::DialogTask)
{
    this->project_id = project_id;
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    if(!task_id)
    {
        this->task = new Task();
    }
    else
    {
        this->task = Task::findById(task_id);
        ui->dateTimeEdit->setDateTime(this->task->date);
        ui->lineEditLabel->setText(this->task->label);
        ui->editor->setContent(this->task->comment);
    }
    ui->lineEditLabel->setFocus();
}

DialogTask::~DialogTask()
{
    delete ui;
}

void DialogTask::on_pushButtonOK_clicked()
{
    this->task->project_id = this->project_id;
    this->task->label = ui->lineEditLabel->text();
    this->task->date = ui->dateTimeEdit->dateTime();
    this->task->comment = ui->editor->content();
    if(!task->id)
        this->task->insert();
    else
        this->task->update();
    accept();
}

void DialogTask::on_pushButtonCancel_clicked()
{
    reject();
}
