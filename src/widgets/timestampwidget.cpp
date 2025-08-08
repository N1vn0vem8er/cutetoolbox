#include "timestampwidget.h"
#include "ui_timestampwidget.h"

TimestampWidget::TimestampWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimestampWidget)
{
    ui->setupUi(this);
    connect(ui->refreshTimeButton, &QPushButton::clicked, this, &TimestampWidget::updateTimestamp);
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &TimestampWidget::setDateTime);
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [&]{ui->currentTime->setText(QString::number(ui->dateTimeEdit->dateTime().currentSecsSinceEpoch()));});
    connect(ui->currentTime, &QLineEdit::editingFinished, this, [&]{ui->dateTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(ui->currentTime->text().toInt()));});
    connect(ui->currentTime, &QLineEdit::textChanged, this, &TimestampWidget::setDateTime);
    updateTimestamp();
    setDateTime();
}

TimestampWidget::~TimestampWidget()
{
    delete ui;
}

void TimestampWidget::updateTimestamp()
{
    ui->currentTime->setText(QString::number(QDateTime::currentDateTime().currentSecsSinceEpoch()));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    setDateTime();
}

void TimestampWidget::setDateTime()
{
    ui->iso8601->setText(ui->dateTimeEdit->dateTime().toString(Qt::ISODate));
    ui->rfc2822->setText(ui->dateTimeEdit->dateTime().toString(Qt::RFC2822Date));
    ui->shortDate->setText(ui->dateTimeEdit->date().toString("dd:MM:yy"));
    ui->shortTime->setText(ui->dateTimeEdit->time().toString("hh:mm:ss"));
    ui->longDate->setText(ui->dateTimeEdit->date().toString("MMMM dd yy"));
    ui->longTime->setText(ui->dateTimeEdit->time().toString("hh:mm:ss"));
    ui->fullTimeDate->setText(ui->dateTimeEdit->dateTime().toUTC().toString("dddd, MMMM, dd yy hh:mm:ss"));
}
