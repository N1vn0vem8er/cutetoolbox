#include "base64coderdecoderwidget.h"
#include "src/widgets/ui_base64coderdecoderwidget.h"

Base64CoderDecoderWidget::Base64CoderDecoderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Base64CoderDecoderWidget)
{
    ui->setupUi(this);
}

Base64CoderDecoderWidget::~Base64CoderDecoderWidget()
{
    delete ui;
}
