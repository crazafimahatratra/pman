#include "rtfeditor.h"
#include "ui_rtfeditor.h"

RtfEditor::RtfEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtfEditor)
{
    ui->setupUi(this);
    setDisplayToolButtons(AllButtons);
}

RtfEditor::~RtfEditor()
{
    delete ui;
}

void RtfEditor::setContent(QString html)
{
    ui->textEdit->setHtml(html);
}

void RtfEditor::setDisplayToolButtons(RtfEditor::ToolButtonFlags mask)
{
    ui->toolButtonSave->setVisible(mask & SaveButton);
    ui->toolButtonBold->setVisible(mask & BoldButton);
    ui->toolButtonItalic->setVisible(mask & ItalicButton);
    ui->toolButtonUnderline->setVisible(mask & UnderlineButton);
    ui->toolButtonTextColor->setVisible(mask & TextColorButton);
    this->displayToolButtonsMask = mask;
}

QString RtfEditor::content()
{
    return ui->textEdit->toHtml();
}

QString RtfEditor::plainText()
{
    return ui->textEdit->toPlainText();
}

void RtfEditor::on_toolButtonBold_clicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    mergeFormat(fmt);
}

void RtfEditor::on_toolButtonItalic_clicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    mergeFormat(fmt);
}

void RtfEditor::on_toolButtonUnderline_clicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    mergeFormat(fmt);
}

void RtfEditor::on_textEdit_cursorPositionChanged()
{
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    ui->toolButtonBold->setChecked(fmt.fontWeight() == QFont::Bold);
    ui->toolButtonItalic->setChecked(fmt.fontItalic());
    ui->toolButtonUnderline->setChecked(fmt.fontUnderline());
}

void RtfEditor::on_toolButtonSave_clicked()
{
    emit this->save(ui->textEdit->toHtml());
}

void RtfEditor::mergeFormat(QTextCharFormat fmt)
{
    if(!ui->textEdit->textCursor().hasSelection())
        return;
    ui->textEdit->textCursor().mergeCharFormat(fmt);
}
