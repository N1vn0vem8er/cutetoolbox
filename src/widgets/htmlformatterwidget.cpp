#include "htmlformatterwidget.h"
#include "src/widgets/ui_htmlformatterwidget.h"

HTMLFormatterWidget::HTMLFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HTMLFormatterWidget)
{
    ui->setupUi(this);
}

HTMLFormatterWidget::~HTMLFormatterWidget()
{
    delete ui;
}
