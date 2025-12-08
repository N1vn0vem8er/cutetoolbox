#include "ipsubnetcalculator.h"
#include "src/widgets/ui_ipsubnetcalculator.h"

#include <QStandardItemModel>
#include <qhostaddress.h>

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
    QHostAddress ipaddress(ui->ipv4LineEdit->text());
    if(ipaddress.isNull() || ipaddress.protocol() != QAbstractSocket::IPv4Protocol)
    {
        return;
    }
    quint32 netmaskValue = 0;
    int cidrPrefix = 0;
    bool ok;
    int inputCidr = ui->ipv4Subnet->text().toInt(&ok);
    if(ok)
    {
        cidrPrefix = inputCidr;
    }
    else
    {
        QHostAddress netmaskHost(ui->ipv4Subnet->text());
        if(netmaskHost.isNull() || netmaskHost.protocol() != QAbstractSocket::IPv4Protocol)
        {
            return;
        }
        netmaskValue = netmaskHost.toIPv4Address();
        quint32 temp = netmaskValue;
        while(temp > 0)
        {
            if((temp & 1) == 1)
            {
                cidrPrefix++;
            }
            temp >>= 1;
        }
        if(cidrPrefix == 0 || (netmaskValue & (~(0xFFFFFFFF >> (32 - cidrPrefix)))) != netmaskValue)
        {
            return;
        }
    }
    if(netmaskValue == 0 && cidrPrefix > 0)
    {
        netmaskValue = 0xFFFFFFFF << (32 - cidrPrefix);
    }
    quint32 networkAddressValue = ipaddress.toIPv4Address() & netmaskValue;
    model->appendRow({new QStandardItem(tr("Ip address")), new QStandardItem(ipaddress.toIPv4Address())});
    model->appendRow({new QStandardItem(tr("Cidi prefix")), new QStandardItem(QString::number(cidrPrefix))});
    quint32 wildcardMask = ~netmaskValue;
    model->appendRow({new QStandardItem(tr("Wildcard mask")), new QStandardItem(QString::number(wildcardMask))});
    model->appendRow({new QStandardItem(tr("Network address")), new QStandardItem(QHostAddress(networkAddressValue).toString())});
    model->appendRow({new QStandardItem(tr("Broadcast address")), new QStandardItem(QHostAddress(networkAddressValue | wildcardMask).toString())});
}

void IpSubnetCalculator::calculateIpv6()
{
    if(ui->resultsTableView->model())
        ui->resultsTableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->resultsTableView);

    ui->resultsTableView->setModel(model);
}
