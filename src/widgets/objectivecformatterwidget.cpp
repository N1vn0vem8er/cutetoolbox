#include "objectivecformatterwidget.h"
#include "src/widgets/ui_objectivecformatterwidget.h"

ObjectiveCFormatterWidget::ObjectiveCFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ObjectiveCFormatterWidget)
{
    ui->setupUi(this);
}

ObjectiveCFormatterWidget::~ObjectiveCFormatterWidget()
{
    delete ui;
}
