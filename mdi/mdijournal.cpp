#include "mdijournal.h"
#include "ui_mdijournal.h"
#include "constants.h"
#include "dialogs/dialogevent.h"
#include "models/event.h"
#include <QToolBar>
#include <QtWidgets/QMessageBox>

MdiJournal::MdiJournal(QDate date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MdiJournal)
{
    this->date = date;
    ui->setupUi(this);
    setWindowTitle(QString("Journal %1").arg(this->date.toString(DATE_FORMAT)));

    QToolBar *toolbar = new QToolBar(ui->frame);
    toolbar->addAction(ui->actionNewEvent);
    toolbar->addAction(ui->actionEditEvent);
    toolbar->addAction(ui->actionDeleteEvent);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fillEvents();
}

MdiJournal::~MdiJournal()
{
    delete ui;
}

QDate MdiJournal::journalDate()
{
    return date;
}

void MdiJournal::on_actionNewEvent_triggered()
{
    DialogEvent *dialog = new DialogEvent(this, this->date);
    if(dialog->exec())
    {
        fillEvents();
        emit this->EventListModified(this->date);
    }
}

void MdiJournal::on_actionDeleteEvent_triggered()
{
    int res = QMessageBox::question(this, "Confirmation", "You sure ?", QMessageBox::Yes | QMessageBox::No);
    if(res == QMessageBox::No)
        return;
    int event_id = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt();
    Event *e = Event::findById(event_id);
    e->remove();
    delete e;
    emit this->EventListModified(this->date);
    fillEvents();
}

Project *MdiJournal::findById(int id)
{
    for(int i = 0; i < this->projects.length(); i++)
    {
        if(projects[i]->id == id)
            return projects[i];
    }
    return 0;
}

void MdiJournal::fillEvents()
{
    this->projects = Project::findAll();
    QList<Event *> rows = Event::findByDate(this->date);
    ui->tableWidget->setRowCount(rows.length());
    for(int i = 0; i < rows.length(); i++)
    {
        Project *p = findById(rows[i]->project_id);
        QTableWidgetItem *item0 = new QTableWidgetItem(p ? p->toIcon() : QIcon(), rows[i]->ctime.toString(TIME_FORMAT));
        item0->setData(Qt::UserRole, rows[i]->id);
        item0->setData(Qt::UserRole + 1, rows[i]->detail);
        item0->setFlags(item0->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(i, 0, item0);
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(rows[i]->title));
        ui->tableWidget->item(i, 1)->setFlags(ui->tableWidget->item(i, 1)->flags() ^ Qt::ItemIsEditable);
    }
    updateToolBar();
}

void MdiJournal::updateToolBar()
{
    bool enabled = ui->tableWidget->selectedItems().count() > 0;
    ui->actionEditEvent->setEnabled(enabled);
    ui->actionDeleteEvent->setEnabled(enabled);
}

void MdiJournal::on_tableWidget_cellClicked(int row, int)
{
    QString detail = ui->tableWidget->item(row, 0)->data(Qt::UserRole + 1).toString();
    ui->textEdit->setHtml(detail);
}

void MdiJournal::on_tableWidget_itemSelectionChanged()
{
    updateToolBar();
}

void MdiJournal::on_actionEditEvent_triggered()
{
    int id = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->data(Qt::UserRole).toInt();
    DialogEvent *d = new DialogEvent(this, this->date, id);
    if(d->exec())
        fillEvents();
}
