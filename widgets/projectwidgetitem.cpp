#include "projectwidgetitem.h"
#include "ui_projectwidgetitem.h"

ProjectWidgetItem::ProjectWidgetItem(QWidget *parent, QString label, int count) :
    QWidget(parent),
    ui(new Ui::ProjectWidgetItem)
{
    ui->setupUi(this);
    ui->label->setText(label);
    ui->labelCount->setText("");
    this->setCount(count);
}

ProjectWidgetItem::~ProjectWidgetItem()
{
    delete ui;
}

void ProjectWidgetItem::setCount(int count)
{
    if(count)
    {
        ui->labelCount->setText(QString("(%1)").arg(count));
    } else {
        ui->labelCount->setText("");
    }
}
