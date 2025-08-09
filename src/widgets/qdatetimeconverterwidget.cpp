#include "qdatetimeconverterwidget.h"
#include "src/widgets/ui_qdatetimeconverterwidget.h"

QDateTimeConverterWidget::QDateTimeConverterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QDateTimeConverterWidget)
{
    ui->setupUi(this);
}

QDateTimeConverterWidget::~QDateTimeConverterWidget()
{
    delete ui;
}
