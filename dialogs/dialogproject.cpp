#include "dialogproject.h"
#include "ui_dialogproject.h"
#include <QtWidgets/QColorDialog>

DialogProject::DialogProject(QWidget *parent, int project_id) :
    QDialog(parent),
    ui(new Ui::DialogProject)
{
    ui->setupUi(this);
    if(project_id)
    {
        project = Project::findById(project_id);
    }
    else
    {
        project = new Project();
    }
    ui->lineEditName->setText(project->name);
    ui->pushButtonColor->setIcon(project->toIcon());
}

DialogProject::~DialogProject()
{
    delete ui;
}

void DialogProject::on_pushButton_2_clicked()
{
    project->name = this->ui->lineEditName->text();
    if(project->id)
    {
        project->update();
    }
    else
    {
        project->insert();
    }
    this->accept();
}

void DialogProject::on_pushButton_clicked()
{
    this->reject();
}

void DialogProject::on_pushButtonColor_clicked()
{
    QColorDialog dialog(this);
    QStringList customColors;

    customColors << "#1abc9c" << "#00AEB4" << "#009CCA" << "#0088D5" << "#006ECE" << "#654DB2";
    customColors << "#2ecc71" << "#00BA75" << "#00A978" << "#009679" << "#008478" << "#007275";
    customColors << "#3498db" << "#00B5E4" << "#00CFD4" << "#2EE4B1" << "#A0F28A" << "#F9F871";
    customColors << "#9b59b6" << "#DF5DA4" << "#FF7388" << "#FF9B6C" << "#FFC95F" << "#F9F871";
    customColors << "#f1c40f" << "#ADBC00" << "#66AF20" << "#009E3B" << "#008A52" << "#007564";
    customColors << "#e67e22" << "#B18700" << "#798900" << "#39851E" << "#007D41" << "#00725F";
    customColors << "#e74c3c" << "#B96B00" << "#7D7E00" << "#2F861F" << "#00865A" << "#00818E";
    customColors << "#34495e" << "#1B6F86" << "#00979A" << "#3BBD96" << "#95DF82" << "#F9F871";

    for(int i = 0; i < customColors.length(); i++)
    {
        dialog.setStandardColor(i, QColor(customColors[i]));
    }
    if(dialog.exec())
    {
        QColor color = dialog.selectedColor();
        project->color = color.name();
        ui->pushButtonColor->setIcon(project->toIcon());
    }
}
