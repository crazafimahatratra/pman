#ifndef RTFEDITOR_H
#define RTFEDITOR_H

#include <QWidget>
#include <QTextCharFormat>

namespace Ui {
class RtfEditor;
}

class RtfEditor : public QWidget
{
    Q_OBJECT

public:
    enum ToolButtonFlags {
        SaveButton      = 1 << 0,
        BoldButton      = 1 << 1,
        ItalicButton    = 1 << 2,
        UnderlineButton = 1 << 3,
        TextColorButton = 1 << 4,
        AllButtons      = 0xFF
    };

    inline friend ToolButtonFlags operator | (ToolButtonFlags a, ToolButtonFlags b)
    {
        return static_cast<ToolButtonFlags>((static_cast<int>(a) | static_cast<int>(b)));
    }
    inline friend ToolButtonFlags operator & (ToolButtonFlags a, ToolButtonFlags b)
    {
        return static_cast<ToolButtonFlags>((static_cast<int>(a) & static_cast<int>(b)));
    }
    inline friend ToolButtonFlags operator ^ (ToolButtonFlags a, ToolButtonFlags b)
    {
        return static_cast<ToolButtonFlags>((static_cast<int>(a) ^ static_cast<int>(b)));
    }

    explicit RtfEditor(QWidget *parent = 0);
    ~RtfEditor();
    void setContent(QString html);
    void setDisplayToolButtons(ToolButtonFlags mask);
    QString content();
    QString plainText();
signals:
    void save(QString html);

private slots:
    void on_toolButtonBold_clicked(bool checked);

    void on_toolButtonItalic_clicked(bool checked);

    void on_toolButtonUnderline_clicked(bool checked);

    void on_textEdit_cursorPositionChanged();

    void on_toolButtonSave_clicked();

private:
    Ui::RtfEditor *ui;
    void mergeFormat(QTextCharFormat fmt);
    ToolButtonFlags displayToolButtonsMask;
};
#endif // RTFEDITOR_H
