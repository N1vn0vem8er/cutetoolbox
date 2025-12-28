#include "domtreewidget.h"
#include "src/widgets/ui_domtreewidget.h"

DomTreeWidget::DomTreeWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::DomTreeWidget)
{
    ui->setupUi(this);
}

DomTreeWidget::~DomTreeWidget()
{
    delete ui;
}
