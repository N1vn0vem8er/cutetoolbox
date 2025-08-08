#include "timestampwidget.h"
#include "ui_timestampwidget.h"

TimestampWidget::TimestampWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimestampWidget)
{
    ui->setupUi(this);
}

TimestampWidget::~TimestampWidget()
{
    delete ui;
}
