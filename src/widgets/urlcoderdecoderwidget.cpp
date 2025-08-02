#include "urlcoderdecoderwidget.h"
#include "src/widgets/ui_urlcoderdecoderwidget.h"

UrlCoderDecoderWidget::UrlCoderDecoderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UrlCoderDecoderWidget)
{
    ui->setupUi(this);
}

UrlCoderDecoderWidget::~UrlCoderDecoderWidget()
{
    delete ui;
}
