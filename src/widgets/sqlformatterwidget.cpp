#include "sqlformatterwidget.h"
#include "src/widgets/ui_sqlformatterwidget.h"

SQLFormatterWidget::SQLFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SQLFormatterWidget)
{
    ui->setupUi(this);
}

SQLFormatterWidget::~SQLFormatterWidget()
{
    delete ui;
}
