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
    bool canSaveFiles() const override;
    void save() override;
    void saveAs() override;
    QString getOpenedFileName() const override;
    void close() override;

private:
    Ui::IpSubnetCalculator *ui;
    QString openedFile;

private:
    QString toCsv() const;

private slots:
    void calculateIpv4();
    void calculateIpv6();
};

#endif // IPSUBNETCALCULATOR_H
