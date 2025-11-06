#include "apitester.h"
#include "src/widgets/ui_apitester.h"

ApiTester::ApiTester(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ApiTester)
{
    ui->setupUi(this);
}

ApiTester::~ApiTester()
{
    delete ui;
}
