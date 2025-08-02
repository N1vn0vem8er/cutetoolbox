#include "jsonschemavalidatorwidget.h"
#include "src/widgets/ui_jsonschemavalidatorwidget.h"

JSONSchemaValidatorWidget::JSONSchemaValidatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JSONSchemaValidatorWidget)
{
    ui->setupUi(this);
}

JSONSchemaValidatorWidget::~JSONSchemaValidatorWidget()
{
    delete ui;
}
