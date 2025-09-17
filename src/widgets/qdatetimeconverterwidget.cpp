#include "qdatetimeconverterwidget.h"
#include "src/widgets/ui_qdatetimeconverterwidget.h"

QDateTimeConverterWidget::QDateTimeConverterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::QDateTimeConverterWidget)
{
    ui->setupUi(this);
    setName(tr("QDateTime to QString Converter"));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->format, &QLineEdit::textChanged, this, &QDateTimeConverterWidget::format);
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &QDateTimeConverterWidget::format);
    connect(ui->getCurrentDateTimeButton, &QPushButton::clicked, this, [&]{ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());});
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->format->selectAll(); ui->format->copy();});
    connect(ui->pasteButton, &QPushButton::clicked, ui->format, &QLineEdit::paste);
    format();
}

QDateTimeConverterWidget::~QDateTimeConverterWidget()
{
    delete ui;
}

bool QDateTimeConverterWidget::canBasicEdit() const
{
    return true;
}

void QDateTimeConverterWidget::format()
{
    ui->dateTime->setText(ui->dateTimeEdit->dateTime().toString(ui->format->text()));
}
