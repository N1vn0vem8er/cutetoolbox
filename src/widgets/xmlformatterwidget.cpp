#include "xmlformatterwidget.h"
#include "src/widgets/ui_xmlformatterwidget.h"

XMLFormatterWidget::XMLFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XMLFormatterWidget)
{
    ui->setupUi(this);
}

XMLFormatterWidget::~XMLFormatterWidget()
{
    delete ui;
}
