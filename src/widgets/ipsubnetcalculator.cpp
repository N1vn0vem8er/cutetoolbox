#include "ipsubnetcalculator.h"
#include "src/widgets/ui_ipsubnetcalculator.h"

#include <QStandardItemModel>

IpSubnetCalculator::IpSubnetCalculator(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::IpSubnetCalculator)
{
    ui->setupUi(this);
    setName("IP Subnet Calculator");
    connect(ui->calculateIPv4Button, &QPushButton::clicked, this, &IpSubnetCalculator::calculateIpv4);
    connect(ui->calculateIPv6Button, &QPushButton::clicked, this, &IpSubnetCalculator::calculateIpv6);
}

IpSubnetCalculator::~IpSubnetCalculator()
{
    delete ui;
}

void IpSubnetCalculator::calculateIpv4()
{
    if(ui->resultsTableView->model())
        ui->resultsTableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->resultsTableView);

    ui->resultsTableView->setModel(model);
}

void IpSubnetCalculator::calculateIpv6()
{
    if(ui->resultsTableView->model())
        ui->resultsTableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->resultsTableView);

    ui->resultsTableView->setModel(model);
}
