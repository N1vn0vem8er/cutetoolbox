#ifndef TIMESTAMPWIDGET_H
#define TIMESTAMPWIDGET_H

#include <QWidget>

namespace Ui {
class TimestampWidget;
}

class TimestampWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimestampWidget(QWidget *parent = nullptr);
    ~TimestampWidget();

private:
    Ui::TimestampWidget *ui;
};

#endif // TIMESTAMPWIDGET_H
