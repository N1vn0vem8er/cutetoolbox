#ifndef MARKDOWNWIDGET_H
#define MARKDOWNWIDGET_H

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
};

#endif // MARKDOWNWIDGET_H
