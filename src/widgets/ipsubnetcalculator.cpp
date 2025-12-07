#include "ipsubnetcalculator.h"
#include "src/widgets/ui_ipsubnetcalculator.h"

IpSubnetCalculator::IpSubnetCalculator(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::IpSubnetCalculator)
{
    ui->setupUi(this);
    setName("IP Subnet Calculator");
}

IpSubnetCalculator::~IpSubnetCalculator()
{
    delete ui;
}
