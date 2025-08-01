#include "hashwidget.h"
#include "src/widgets/ui_hashwidget.h"

HashWidget::HashWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HashWidget)
{
    ui->setupUi(this);
}

HashWidget::~HashWidget()
{
    delete ui;
}
