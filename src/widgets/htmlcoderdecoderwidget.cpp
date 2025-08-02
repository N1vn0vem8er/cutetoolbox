#include "htmlcoderdecoderwidget.h"
#include "src/widgets/ui_htmlcoderdecoderwidget.h"

HTMLCoderDecoderWidget::HTMLCoderDecoderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HTMLCoderDecoderWidget)
{
    ui->setupUi(this);
}

HTMLCoderDecoderWidget::~HTMLCoderDecoderWidget()
{
    delete ui;
}
