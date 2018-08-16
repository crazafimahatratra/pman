#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMdiArea>
#include <QDate>
#include <QSystemTrayIcon>
#include "dialogs/dialogminitasks.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void changeEvent(QEvent *event);
signals:
    void projectRemoved(int);
    void projectModified(int);

private slots:
    void on_action_Project_triggered();

    void on_action_Quitter_triggered();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_actionEditProject_triggered();

    void on_actionDeleteProject_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_calendar_activated(const QDate &date);

    void on_calendar_currentPageChanged(int year, int month);

    void onEventAdded(QDate date);

    void on_actionTasks_triggered(bool checked);

    void on_actionEvents_triggered(bool checked);

    void onSystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void onMiniWindowClosed();

    void onTaskListChanged();


    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *system_icon;
    DialogMiniTasks *dialogMiniTasks;
    void updateToolBar();
    void listProjects();
    void updateMdiTabBar(QMdiArea *area);
    void highlightDates(int year, int month);
};

#endif // MAINWINDOW_H
