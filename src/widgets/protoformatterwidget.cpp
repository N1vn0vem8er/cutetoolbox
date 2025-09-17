#include "protoformatterwidget.h"
#include "src/widgets/ui_protoformatterwidget.h"

ProtoFormatterWidget::ProtoFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ProtoFormatterWidget)
{
    ui->setupUi(this);
}

ProtoFormatterWidget::~ProtoFormatterWidget()
{
    delete ui;
}
