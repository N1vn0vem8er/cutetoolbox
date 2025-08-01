#include "loremipsumgenerator.h"
#include "src/widgets/ui_loremipsumgenerator.h"

LoremIpsumGenerator::LoremIpsumGenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoremIpsumGenerator)
{
    ui->setupUi(this);
}

LoremIpsumGenerator::~LoremIpsumGenerator()
{
    delete ui;
}
