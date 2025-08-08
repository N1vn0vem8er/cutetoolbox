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
    connect(ui->copyISO8601Button, &QPushButton::clicked, this, [&]{ui->iso8601->selectAll(); ui->iso8601->copy();});
    connect(ui->copyRFC2822Button, &QPushButton::clicked, this, [&]{ui->rfc2822->selectAll(); ui->rfc2822->copy();});
    connect(ui->copyShortTimeButton, &QPushButton::clicked, this, [&]{ui->shortTime->selectAll(); ui->shortTime->copy();});
    connect(ui->copyShortDateButton, &QPushButton::clicked, this, [&]{ui->shortDate->selectAll(); ui->shortDate->copy();});
    connect(ui->copyLongDateButton, &QPushButton::clicked, this, [&]{ui->longDate->selectAll(); ui->longDate->copy();});
    connect(ui->copyLongTimeButton, &QPushButton::clicked, this, [&]{ui->longTime->selectAll(); ui->longTime->copy();});
    connect(ui->copyFullDateButton, &QPushButton::clicked, this, [&]{ui->fullTimeDate->selectAll(); ui->fullTimeDate->copy();});
    connect(ui->copyCurrentTimeButton, &QPushButton::clicked, this, [&]{ui->currentTime->selectAll(); ui->currentTime->copy();});
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
