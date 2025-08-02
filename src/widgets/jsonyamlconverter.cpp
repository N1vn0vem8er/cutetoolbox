#include "jsonyamlconverter.h"
#include "src/widgets/ui_jsonyamlconverter.h"

JsonYamlConverter::JsonYamlConverter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JsonYamlConverter)
{
    ui->setupUi(this);
}

JsonYamlConverter::~JsonYamlConverter()
{
    delete ui;
}
