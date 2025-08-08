#include "chmodcalculatorwidget.h"
#include "src/widgets/ui_chmodcalculatorwidget.h"

ChmodCalculatorWidget::ChmodCalculatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChmodCalculatorWidget)
{
    ui->setupUi(this);
    connect(ui->readOwner, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->writeOwner, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->runOwner, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->readGroup, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->writeGroup, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->runGroup, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->readOther, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->writeOther, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->runOther, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
}

ChmodCalculatorWidget::~ChmodCalculatorWidget()
{
    delete ui;
}

void ChmodCalculatorWidget::generate()
{
    int userPermissions = 0;
    if(ui->readOwner->isChecked())
        userPermissions+=4;
    if(ui->writeOwner->isChecked())
        userPermissions+=2;
    if(ui->runOwner->isChecked())
        userPermissions+=1;
    int groupPermissions = 0;
    if(ui->readGroup->isChecked())
        groupPermissions+=4;
    if(ui->writeGroup->isChecked())
        groupPermissions+=2;
    if(ui->runGroup->isChecked())
        groupPermissions+=1;
    int otherPermissions = 0;
    if(ui->readOther->isChecked())
        otherPermissions+=4;
    if(ui->writeOther->isChecked())
        otherPermissions+=2;
    if(ui->runOther->isChecked())
        otherPermissions+=1;
    ui->numeric->setText(QString("%1%2%3").arg(userPermissions).arg(groupPermissions).arg(otherPermissions));
    auto getSymbolic = [&](int permissions){
        QString result = "";
        result += (permissions & 4) ? "r" : "-";
        result += (permissions & 2) ? "w" : "-";
        result += (permissions & 1) ? "x" : "-";
        return result;
    };
    ui->symbolic->setText(getSymbolic(userPermissions) + getSymbolic(groupPermissions) + getSymbolic(otherPermissions));
}

void ChmodCalculatorWidget::updateCheckboxes()
{

}
