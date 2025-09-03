#ifndef TIMESTAMPWIDGET_H
#define TIMESTAMPWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class TimestampWidget;
}

class TimestampWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit TimestampWidget(QWidget *parent = nullptr);
    ~TimestampWidget();
    bool canBasicEdit() const override;

private:
    Ui::TimestampWidget *ui;

private slots:
    void updateTimestamp();
    void setDateTime();
};

#endif // TIMESTAMPWIDGET_H
