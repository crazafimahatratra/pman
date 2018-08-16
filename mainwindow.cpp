#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/dialogproject.h"
#include "dialogabout.h"
#include "models/project.h"
#include "models/event.h"
#include "mdi/mdiproject.h"
#include "mdi/mdijournal.h"
#include <QDebug>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QTextCharFormat>
#include "widgets/projectwidgetitem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    listProjects();
    highlightDates(ui->calendar->yearShown(), ui->calendar->monthShown());
    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->actionTasks);
    group->addAction(ui->actionEvents);
    ui->stackedWidget->setCurrentIndex(0);
    ui->actionTasks->setChecked(true);
    system_icon = new QSystemTrayIcon(QIcon(":/std/icons/87457.ico"), this);
    dialogMiniTasks = new DialogMiniTasks(this);
    connect(system_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(dialogMiniTasks, SIGNAL(miniWindowClosed()), this, SLOT(onMiniWindowClosed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        if(isMinimized())
        {
            system_icon->show();
            this->hide();
            dialogMiniTasks->show();
        }
    }
    event->accept();
}

void MainWindow::on_action_Project_triggered()
{
    DialogProject d(this);
    if(d.exec())
    {
        listProjects();
    }
}

void MainWindow::on_action_Quitter_triggered()
{
    this->close();
}

void MainWindow::listProjects()
{
    QList<Project *> projects = Project::findAll();
    ui->listWidget->clear();
    for(int i = 0; i < projects.length(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setData(Qt::UserRole, projects[i]->id);
        item->setIcon(projects[i]->toIconBullet());
        ProjectWidgetItem *row = new ProjectWidgetItem(0, projects[i]->name, projects[i]->nb_pendings);
        item->setSizeHint(row->minimumSizeHint());
        ui->listWidget->setItemWidget(item, row);
        ui->listWidget->addItem(item);
    }
    updateToolBar();
}

void MainWindow::updateMdiTabBar(QMdiArea *area)
{
    QList<QTabBar*> bars = area->findChildren<QTabBar*>("", Qt::FindDirectChildrenOnly);
    for(int i = 0; i < bars.length(); i++)
    {
        bars[i]->setExpanding(false);
    }
}

void MainWindow::highlightDates(int year, int month)
{
    QDate begin(ui->calendar->yearShown(), ui->calendar->monthShown(), 1);
    QDate end = begin.addMonths(1).addDays(-1);
    for(QDate d = begin; d <= end; d = d.addDays(1))
    {
        QTextCharFormat format = ui->calendar->dateTextFormat(d);
        format.setFontWeight(QFont::Normal);
        format.setFontPointSize(8);
        format.setFontUnderline(false);
        ui->calendar->setDateTextFormat(d, format);
    }

    QList<Event *> events = Event::findByMonth(year, month);
    for(int i = 0; i < events.length(); i++)
    {
        QDate d = events[i]->ctime.date();
        QTextCharFormat format = ui->calendar->dateTextFormat(d);
        format.setFontWeight(QFont::Bold);
        format.setFontPointSize(9);
        format.setFontUnderline(true);
        ui->calendar->setDateTextFormat(d, format);
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->actionTasks->setChecked(true);
    int id = item->data(Qt::UserRole).toInt();
    QList<QMdiSubWindow*> subwindows = ui->mdiAreaProjects->subWindowList();
    for(int i = 0; i < subwindows.length(); i++)
    {
        MdiProject* p = (MdiProject*)(subwindows[i]->widget());
        if(id == p->projectId())
        {
            ui->mdiAreaProjects->setActiveSubWindow(subwindows[i]);
            return;
        }
    }

    MdiProject *win = new MdiProject(this, id);
    ui->mdiAreaProjects->addSubWindow(win);
    win->showMaximized();
    connect(win, SIGNAL(TaskListModified()), this, SLOT(onTaskListChanged()));
    updateMdiTabBar(ui->mdiAreaProjects);
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget->itemAt(pos);
    if(item == 0)
        return;
    updateToolBar();
    QMenu *menu = new QMenu(ui->listWidget);
    menu->addAction(ui->actionDeleteProject);
    menu->addAction(ui->actionEditProject);
    menu->popup(ui->listWidget->mapToGlobal(pos));
}

void MainWindow::on_actionEditProject_triggered()
{
    if(!ui->listWidget->currentItem())
        return;
    int project_id = ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
    DialogProject d(this, project_id);
    if(d.exec())
    {
        listProjects();
        emit this->projectModified(project_id);
    }
}

void MainWindow::on_actionDeleteProject_triggered()
{
    if(!ui->listWidget->currentItem())
        return;
    int ret = QMessageBox::question(this, "Confirmation", "You sure ?", QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::No)
        return;
    int project_id = ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
    Project *p = Project::findById(project_id);
    if(p)
    {
        p->remove();
        emit this->projectRemoved(project_id);
        listProjects();
    }
}

void MainWindow::updateToolBar()
{
    bool enabled = ui->listWidget->currentItem();
    ui->actionEditProject->setEnabled(enabled);
    ui->actionDeleteProject->setEnabled(enabled);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *)
{
    updateToolBar();
}

void MainWindow::on_calendar_activated(const QDate &date)
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->actionEvents->setChecked(true);
    QList<QMdiSubWindow*> subwindows = ui->mdiAreaJournals->subWindowList();
    for(int i = 0; i < subwindows.length(); i++)
    {
        MdiJournal* p = (MdiJournal*)(subwindows[i]->widget());
        if(p->journalDate() == date)
        {
            ui->mdiAreaJournals->setActiveSubWindow(subwindows[i]);
            return;
        }
    }

    MdiJournal *journal = new MdiJournal(date);
    connect(journal, SIGNAL(EventListModified(QDate)), this, SLOT(onEventAdded(QDate)));
    ui->mdiAreaJournals->addSubWindow(journal);
    journal->showMaximized();
    updateMdiTabBar(ui->mdiAreaJournals);
}

void MainWindow::on_calendar_currentPageChanged(int year, int month)
{
    highlightDates(year, month);
}

void MainWindow::onEventAdded(QDate date)
{
    highlightDates(date.year(), date.month());
}

void MainWindow::on_actionTasks_triggered(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_actionEvents_triggered(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        system_icon->hide();
        this->showNormal();
        this->activateWindow();
        dialogMiniTasks->hide();
    }
}

void MainWindow::onMiniWindowClosed()
{
    this->showNormal();
    this->activateWindow();
    dialogMiniTasks->hide();
}

void MainWindow::onTaskListChanged()
{
    listProjects();
}



void MainWindow::on_actionAbout_triggered()
{
    DialogAbout *about = new DialogAbout(this);
    about->exec();
}
