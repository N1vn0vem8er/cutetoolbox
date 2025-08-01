#include "qrcodegenerator.h"
#include "src/widgets/ui_qrcodegenerator.h"

QrCodeGenerator::QrCodeGenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QrCodeGenerator)
{
    ui->setupUi(this);
}

QrCodeGenerator::~QrCodeGenerator()
{
    delete ui;
}
