#include "qdatetimeconverterwidget.h"
#include "src/widgets/ui_qdatetimeconverterwidget.h"

QDateTimeConverterWidget::QDateTimeConverterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QDateTimeConverterWidget)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->format, &QLineEdit::textChanged, this, &QDateTimeConverterWidget::format);
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &QDateTimeConverterWidget::format);
    connect(ui->getCurrentDateTimeButton, &QPushButton::clicked, this, [&]{ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());});
    format();
}

QDateTimeConverterWidget::~QDateTimeConverterWidget()
{
    delete ui;
}

void QDateTimeConverterWidget::format()
{
    ui->dateTime->setText(ui->dateTimeEdit->dateTime().toString(ui->format->text()));
}
