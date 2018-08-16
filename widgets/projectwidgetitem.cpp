#include "projectwidgetitem.h"
#include "ui_projectwidgetitem.h"

ProjectWidgetItem::ProjectWidgetItem(QWidget *parent, QString label, int count) :
    QWidget(parent),
    ui(new Ui::ProjectWidgetItem)
{
    ui->setupUi(this);
    ui->label->setText(label);
    ui->labelCount->setText("");
    if(count) {
        ui->labelCount->setText(QString("(%1)").arg(count));
    }
}

ProjectWidgetItem::~ProjectWidgetItem()
{
    delete ui;
}
