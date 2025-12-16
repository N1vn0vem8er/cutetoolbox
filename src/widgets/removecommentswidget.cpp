#include "removecommentswidget.h"
#include "src/widgets/ui_removecommentswidget.h"

RemoveCommentsWidget::RemoveCommentsWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RemoveCommentsWidget)
{
    ui->setupUi(this);
}

RemoveCommentsWidget::~RemoveCommentsWidget()
{
    delete ui;
}
