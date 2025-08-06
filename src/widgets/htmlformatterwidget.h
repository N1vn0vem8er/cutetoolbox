#ifndef HTMLFORMATTERWIDGET_H
#define HTMLFORMATTERWIDGET_H

#include "syntaxhighlighters/htmlsyntaxhighlighter.h"
#include <QWidget>

namespace Ui {
class HTMLFormatterWidget;
}

class HTMLFormatterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HTMLFormatterWidget(QWidget *parent = nullptr);
    ~HTMLFormatterWidget();

private:
    Ui::HTMLFormatterWidget *ui;
    HTMLSyntaxHighlighter* syntaxHighlighter {nullptr};

private slots:
    void format();
    void open();
};

#endif // HTMLFORMATTERWIDGET_H
