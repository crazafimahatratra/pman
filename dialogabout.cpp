#include "dialogabout.h"
#include "ui_dialogabout.h"
#include <QFile>
#include <QTextStream>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    QString lic_fugue = readlicense(":/licenses/license_fugue.txt");
    QString lic_qt = readlicense(":/licenses/license_qt.txt");
    ui->textBrowserLicense->setHtml(QString("<h1>Fugue icons</h1><pre>%1</pre><h1>Qt</h1><pre>%2</pre>").arg(lic_fugue).arg(lic_qt));
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

QString DialogAbout::readlicense(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        return "ERROR";
    }
    QTextStream in(&file);
    QString res = in.readAll();
    file.close();
    return res;
}
