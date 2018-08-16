#ifndef MDIJOURNAL_H
#define MDIJOURNAL_H

#include <QDialog>
#include <QDate>
#include "models/project.h"

namespace Ui {
class MdiJournal;
}

class MdiJournal : public QDialog
{
    Q_OBJECT

public:
    explicit MdiJournal(QDate date, QWidget *parent = 0);
    ~MdiJournal();
    QDate journalDate();
signals:
    void EventListModified(QDate date);

private slots:
    void on_actionNewEvent_triggered();

    void on_actionDeleteEvent_triggered();

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_itemSelectionChanged();

    void on_actionEditEvent_triggered();

private:
    Ui::MdiJournal *ui;
    QDate date;
    QList<Project *> projects;
    Project * findById(int id);
    void fillEvents();
    void updateToolBar();
};

#endif // MDIJOURNAL_H
