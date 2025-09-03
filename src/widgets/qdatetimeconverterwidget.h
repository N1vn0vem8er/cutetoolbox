#ifndef QDATETIMECONVERTERWIDGET_H
#define QDATETIMECONVERTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class QDateTimeConverterWidget;
}

class QDateTimeConverterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit QDateTimeConverterWidget(QWidget *parent = nullptr);
    ~QDateTimeConverterWidget();
    bool canBasicEdit() const override;

private:
    Ui::QDateTimeConverterWidget *ui;

private slots:
    void format();
};

#endif // QDATETIMECONVERTERWIDGET_H
