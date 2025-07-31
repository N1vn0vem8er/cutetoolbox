#include "jsonformatterwidget.h"
#include "src/widgets/ui_jsonformatterwidget.h"

JsonFormatterWidget::JsonFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JsonFormatterWidget)
{
    ui->setupUi(this);
}

JsonFormatterWidget::~JsonFormatterWidget()
{
    delete ui;
}
