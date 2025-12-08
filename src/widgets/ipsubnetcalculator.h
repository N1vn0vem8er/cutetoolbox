#ifndef IPSUBNETCALCULATOR_H
#define IPSUBNETCALCULATOR_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class IpSubnetCalculator;
}

class IpSubnetCalculator : public CustomWidget
{
    Q_OBJECT

public:
    explicit IpSubnetCalculator(QWidget *parent = nullptr);
    ~IpSubnetCalculator();

private:
    Ui::IpSubnetCalculator *ui;

private slots:
    void calculateIpv4();
    void calculateIpv6();
};

#endif // IPSUBNETCALCULATOR_H
