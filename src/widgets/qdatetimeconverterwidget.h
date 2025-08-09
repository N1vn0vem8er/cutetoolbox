#ifndef QDATETIMECONVERTERWIDGET_H
#define QDATETIMECONVERTERWIDGET_H

#include <QWidget>

namespace Ui {
class QDateTimeConverterWidget;
}

class QDateTimeConverterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QDateTimeConverterWidget(QWidget *parent = nullptr);
    ~QDateTimeConverterWidget();

private:
    Ui::QDateTimeConverterWidget *ui;
};

#endif // QDATETIMECONVERTERWIDGET_H
