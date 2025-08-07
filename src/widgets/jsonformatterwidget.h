#ifndef JSONFORMATTERWIDGET_H
#define JSONFORMATTERWIDGET_H

#include "syntaxhighlighters/jsonsyntaxhighlighter.h"
#include <QWidget>

namespace Ui {
class JsonFormatterWidget;
}

class JsonFormatterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JsonFormatterWidget(QWidget *parent = nullptr);
    ~JsonFormatterWidget();

private:
    Ui::JsonFormatterWidget *ui;
    JSONSyntaxHighlighter* syntaxHighlighter {nullptr};

private slots:
    void format();
    void open();
};

#endif // JSONFORMATTERWIDGET_H
