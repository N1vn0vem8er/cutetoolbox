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
        if(cidrPrefix == 0 || (0xFFFFFFFF << (32 - cidrPrefix)) != netmaskValue)
        {
            return;
        }
    }
    if(netmaskValue == 0 && cidrPrefix > 0)
    {
        netmaskValue = 0xFFFFFFFF << (32 - cidrPrefix);
    }
    const quint32 ipValue = ipaddress.toIPv4Address();
    const int hostBits = 32 - cidrPrefix;
    const quint32 networkAddressValue = ipValue & netmaskValue;
    const quint32 wildcardMask = ~netmaskValue;
    const quint32 broadcastAddressValue = networkAddressValue | wildcardMask;
    const qint64 totalHosts = qint64(1) << hostBits;
    qint64 usableHosts = 0;
    QChar ipClass = 'E';
    const quint8 firstOctet = (ipValue >> 24) & 0xFF;
    if(firstOctet >= 1 && firstOctet <= 126)
        ipClass = 'A';
    else if(firstOctet >= 128 && firstOctet <= 191)
        ipClass = 'B';
    else if(firstOctet >= 192 && firstOctet <= 223)
        ipClass = 'C';
    else if(firstOctet >= 224 && firstOctet <= 239)
        ipClass = 'D';
    const QString binarySubnetMask = QString("%1").arg(netmaskValue, 32, 2, QChar('0')).insert(8, '.').insert(17, '.').insert(26, '.');
    QString ipType;
    if(ipValue == networkAddressValue)
        ipType = tr("Network Address");
    else if(ipValue == broadcastAddressValue)
        ipType = tr("Broadcast Address");
    else if((firstOctet == 10) || (firstOctet == 172 && ((ipValue >> 16) & 0xFF) >= 16 && ((ipValue >> 16) & 0xFF) <= 31) || (firstOctet == 192 && ((ipValue >> 16) & 0xFF) == 168))
        ipType = tr("Private");
    else
        ipType = tr("Public");
    const QString binaryID = QString("%1").arg(ipValue, 32, 2, QChar('0')).insert(8, '.').insert(17, '.').insert(26, '.');
    const QString hexID = QString("0x%1").arg(ipValue, 8, 16, QChar('0')).toUpper();

    if(hostBits >= 2)
    {
        usableHosts = totalHosts - 2;
    }
    else if(hostBits == 1)
    {
        usableHosts = 0;
    }
    else
    {
        usableHosts = 0;
    }
    model->appendRow({new QStandardItem(tr("Ip address")), new QStandardItem(ui->ipv4LineEdit->text())});
    model->appendRow({new QStandardItem(tr("Cidi prefix")), new QStandardItem(QString::number(cidrPrefix))});
    model->appendRow({new QStandardItem(tr("Wildcard mask")), new QStandardItem(QHostAddress(wildcardMask).toString())});
    model->appendRow({new QStandardItem(tr("Network address")), new QStandardItem(QHostAddress(networkAddressValue).toString())});
    model->appendRow({new QStandardItem(tr("First address")), new QStandardItem(QHostAddress(networkAddressValue + 1).toString())});
    model->appendRow({new QStandardItem(tr("Last address")), new QStandardItem(QHostAddress(broadcastAddressValue - 1).toString())});
    model->appendRow({new QStandardItem(tr("Total hosts")), new QStandardItem(QString::number(totalHosts))});
    model->appendRow({new QStandardItem(tr("Usable hosts")), new QStandardItem(QString::number(usableHosts))});
    model->appendRow({new QStandardItem(tr("IP Class")), new QStandardItem(QString(ipClass))});
    model->appendRow({new QStandardItem(tr("Binary Subnet Mask")), new QStandardItem(binarySubnetMask)});
    model->appendRow({new QStandardItem(tr("IP Type")), new QStandardItem(ipType)});
    model->appendRow({new QStandardItem(tr("Binary ID")), new QStandardItem(binaryID)});
    model->appendRow({new QStandardItem(tr("Integer ID")), new QStandardItem(QString::number(ipValue))});
    model->appendRow({new QStandardItem(tr("Hex ID")), new QStandardItem(hexID)});
}

void IpSubnetCalculator::calculateIpv6()
{
    if(ui->resultsTableView->model())
        ui->resultsTableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->resultsTableView);
    ui->resultsTableView->setModel(model);
    QHostAddress ipaddress(ui->ipv6LineEdit->text());
    bool ok;
    int cidrPrefix = ui->ipv6PrefixLineEdit->text().toInt(&ok);
    if(!ok || cidrPrefix < 0 || cidrPrefix > 128)
        return;
    model->appendRow({new QStandardItem(tr("Ip address")), new QStandardItem(ui->ipv6LineEdit->text())});
    model->appendRow({new QStandardItem(tr("Cidi prefix")), new QStandardItem(QString::number(cidrPrefix))});
    model->appendRow({new QStandardItem(tr("Address Type")), new QStandardItem(ipaddress.isGlobal() ? tr("Global Unicast") : ipaddress.isLinkLocal()
                                                                                                                             ? tr("Link-Local") : ipaddress.isSiteLocal()
                                                                                                                             ? tr("Unique Local (Deprecated)") : ipaddress.isUniqueLocalUnicast()
                                                                                                                             ? tr("Unique Local Unicast (ULA)") : ipaddress.isMulticast()
                                                                                                                             ? tr("Multicast") : tr("Other/Unknown"))});
}
