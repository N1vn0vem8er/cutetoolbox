#include "numberbaseswidget.h"
#include "src/widgets/ui_numberbaseswidget.h"

NumberBasesWidget::NumberBasesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NumberBasesWidget)
{
    ui->setupUi(this);
}

NumberBasesWidget::~NumberBasesWidget()
{
    delete ui;
}
