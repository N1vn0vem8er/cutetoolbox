#ifndef MARKDOWNWIDGET_H
#define MARKDOWNWIDGET_H

#include "document.h"
#include <QWidget>

namespace Ui {
class MarkdownWidget;
}

class MarkdownWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownWidget(QWidget *parent = nullptr);
    ~MarkdownWidget();

private:
    Ui::MarkdownWidget *ui;
    Document document;

private slots:
    void displayDocument();
};

#endif // MARKDOWNWIDGET_H
