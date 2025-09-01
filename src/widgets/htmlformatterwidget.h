#ifndef HTMLFORMATTERWIDGET_H
#define HTMLFORMATTERWIDGET_H

#include "customwidget.h"
#include "syntaxhighlighters/htmlsyntaxhighlighter.h"
#include <QWidget>

namespace Ui {
class HTMLFormatterWidget;
}

class HTMLFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit HTMLFormatterWidget(QWidget *parent = nullptr);
    ~HTMLFormatterWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void deleteText() override;
    void deleteAllText() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;

private:
    Ui::HTMLFormatterWidget *ui;
    HTMLSyntaxHighlighter* syntaxHighlighter {nullptr};

private slots:
    void format();
};

#endif // HTMLFORMATTERWIDGET_H
