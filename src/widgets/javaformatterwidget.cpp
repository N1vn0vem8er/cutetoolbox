#include "javaformatterwidget.h"
#include "src/widgets/ui_javaformatterwidget.h"

JavaFormatterWidget::JavaFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JavaFormatterWidget)
{
    ui->setupUi(this);
}

JavaFormatterWidget::~JavaFormatterWidget()
{
    delete ui;
}
