#ifndef DIALOGPROJECT_H
#define DIALOGPROJECT_H

#include <QDialog>
#include "models/project.h"

namespace Ui {
class DialogProject;
}

class DialogProject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProject(QWidget *parent = 0, int project_id = 0);
    ~DialogProject();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButtonColor_clicked();

private:
    Ui::DialogProject *ui;
    Project *project;
};

#endif // DIALOGPROJECT_H
