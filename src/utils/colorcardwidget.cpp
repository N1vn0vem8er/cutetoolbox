#include "colorcardwidget.h"
#include "ui_colorcardwidget.h"
#include <qpainter.h>
#include <qstyleoption.h>

ColorCardWidget::ColorCardWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorCardWidget)
{
    ui->setupUi(this);
    connect(ui->lockButton, &QPushButton::clicked, this, &ColorCardWidget::lockOrUnlock);
    connect(ui->removeButton, &QPushButton::clicked, this, &ColorCardWidget::remove);
}

ColorCardWidget::~ColorCardWidget()
{
    delete ui;
}

void ColorCardWidget::setColor(const QColor & newColor)
{
    if(isLocked()) return;
    color = newColor;
    update();
}

QColor ColorCardWidget::getColor() const
{
    return color;
}

bool ColorCardWidget::isLocked() const
{
    return locked;
}

void ColorCardWidget::update()
{
    setStyleSheet(QString("background-color: %1;").arg(color.name(QColor::HexArgb)));
}

void ColorCardWidget::lockOrUnlock()
{
    if(isLocked())
    {
        locked = false;
        ui->lockButton->setText(tr("Lock"));
    }
    else
    {
        locked = true;
        ui->lockButton->setText(tr("Unlock"));
    }
}

void ColorCardWidget::remove()
{
    emit removeColor(this);
}

void ColorCardWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
