#include "xmlschemevalidatorwidget.h"
#include "src/widgets/ui_xmlschemevalidatorwidget.h"

XMLSchemeValidatorWidget::XMLSchemeValidatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XMLSchemeValidatorWidget)
{
    ui->setupUi(this);
}

XMLSchemeValidatorWidget::~XMLSchemeValidatorWidget()
{
    delete ui;
}
