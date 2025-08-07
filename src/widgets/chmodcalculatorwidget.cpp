#include "chmodcalculatorwidget.h"
#include "src/widgets/ui_chmodcalculatorwidget.h"

ChmodCalculatorWidget::ChmodCalculatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChmodCalculatorWidget)
{
    ui->setupUi(this);
}

ChmodCalculatorWidget::~ChmodCalculatorWidget()
{
    delete ui;
}
