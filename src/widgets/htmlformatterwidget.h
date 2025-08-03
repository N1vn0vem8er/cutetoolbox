#ifndef HTMLFORMATTERWIDGET_H
#define HTMLFORMATTERWIDGET_H

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

private slots:
    void format();
    void open();
};

#endif // HTMLFORMATTERWIDGET_H
