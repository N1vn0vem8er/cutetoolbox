#ifndef SQLFORMATTERWIDGET_H
#define SQLFORMATTERWIDGET_H

#include <QWidget>

namespace Ui {
class SQLFormatterWidget;
}

class SQLFormatterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SQLFormatterWidget(QWidget *parent = nullptr);
    ~SQLFormatterWidget();

private:
    Ui::SQLFormatterWidget *ui;
};

#endif // SQLFORMATTERWIDGET_H
