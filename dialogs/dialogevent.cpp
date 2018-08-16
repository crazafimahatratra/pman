#include "dialogevent.h"
#include "ui_dialogevent.h"
#include <QComboBox>
#include "models/project.h"
#include <QDebug>

DialogEvent::DialogEvent(QWidget *parent, QDate date, int event_id) :
    QDialog(parent),
    ui(new Ui::DialogEvent)
{
    ui->setupUi(this);
    fillProjects();
    ui->dateTimeEdit->setDate(date);
    ui->dateTimeEdit->setTime(QDateTime::currentDateTime().time());
    if(event_id)
    {
        this->event = Event::findById(event_id);
        ui->dateTimeEdit->setDateTime(event->ctime);
        ui->lineEditTitle->setText(event->title);
        ui->details->setContent(event->detail);
        for(int i = 0; i < ui->comboBoxProject->count(); i++)
        {
            if(event->project_id == ui->comboBoxProject->itemData(i, Qt::UserRole).toInt())
            {
                ui->comboBoxProject->setCurrentIndex(i);
                break;
            }
        }
    }
    else
    {
        this->event = new Event();
    }
}

DialogEvent::~DialogEvent()
{
    delete ui;
}

void DialogEvent::on_pushButtonOK_clicked()
{
    int pid = ui->comboBoxProject->currentData().toInt();
    event->title = ui->lineEditTitle->text();
    event->ctime = ui->dateTimeEdit->dateTime();
    event->detail = ui->details->content();
    event->project_id = pid;
    if(event->id)
    {
        event->update();
    }
    else
    {
        event->insert();
    }
    delete event;
    accept();
}

void DialogEvent::fillProjects()
{
    ui->comboBoxProject->clear();
    QList<Project*> projects = Project::findAll();
    for(int i = 0; i < projects.length(); i++)
    {
        ui->comboBoxProject->addItem(projects[i]->toIcon(), projects[i]->name, projects[i]->id);
    }
}

void DialogEvent::on_pushButtonCancel_clicked()
{
    reject();
}
