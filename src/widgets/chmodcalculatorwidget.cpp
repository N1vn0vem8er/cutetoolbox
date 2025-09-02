#include "chmodcalculatorwidget.h"
#include "src/widgets/ui_chmodcalculatorwidget.h"

ChmodCalculatorWidget::ChmodCalculatorWidget(QWidget *parent)
    : CustomWidget(parent)
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
    connect(ui->setuid, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->setgui, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->sticky, &QCheckBox::clicked, this, &ChmodCalculatorWidget::generate);
    connect(ui->numeric, &QLineEdit::editingFinished, this, &ChmodCalculatorWidget::updateCheckboxesNumeric);
    connect(ui->symbolic, &QLineEdit::editingFinished, this, &ChmodCalculatorWidget::updateCheckBoxesSymbolic);
    connect(ui->copyNumericButton, &QPushButton::clicked, this, [&]{ui->numeric->selectAll(); ui->numeric->copy();});
    connect(ui->pasteNumericButton, &QPushButton::clicked, ui->numeric, &QLineEdit::paste);
    connect(ui->clearNumericButton, &QPushButton::clicked, ui->numeric, &QLineEdit::clear);
    connect(ui->copySymbolicButton, &QPushButton::clicked, this, [&]{ui->symbolic->selectAll(); ui->symbolic->copy();});
    connect(ui->pasteSymbolicButton, &QPushButton::clicked, ui->symbolic, &QLineEdit::paste);
    connect(ui->clearSymbolicButton, &QPushButton::clicked, ui->symbolic, &QLineEdit::clear);
}

ChmodCalculatorWidget::~ChmodCalculatorWidget()
{
    delete ui;
}

bool ChmodCalculatorWidget::canBasicEdit() const
{
    return true;
}

void ChmodCalculatorWidget::deleteText()
{

}

void ChmodCalculatorWidget::deleteAllText()
{

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
    int special = 0;
    if(ui->setuid->isChecked())
        special+=4;
    if(ui->setgui->isChecked())
        special+=2;
    if(ui->sticky->isChecked())
        special+=1;
    ui->numeric->setText(QString("%1%2%3").arg(userPermissions).arg(groupPermissions).arg(otherPermissions));
    if(special > 0)
        ui->numeric->setText(QString("%1%2").arg(special).arg(ui->numeric->text()));
    enum Type{
        User,
        Group,
        Other
    };
    auto getSymbolic = [&](int permissions, int specialPermissions, Type type){
        QString result = "";
        result += (permissions & 4) ? "r" : "-";
        result += (permissions & 2) ? "w" : "-";
        if(permissions & 1)
        {
            if(type == Type::User && (specialPermissions & 4))
            {
                result += "s";
            }
            else if(type == Type::Group && (specialPermissions & 2))
            {
                result += "s";
            }
            else if(type == Type::Other && (specialPermissions & 1))
            {
                result += "t";
            }
            else
            {
                result += "x";
            }
        }
        else
        {
            if(type == Type::User && (specialPermissions & 4))
            {
                result += "S";
            }
            else if(type == Type::Group && (specialPermissions & 2))
            {
                result += "S";
            }
            else if(type == Type::Other && (specialPermissions & 1))
            {
                result += "T";
            }
            else
            {
                result += "-";
            }
        }
        return result;
    };
    ui->symbolic->setText(getSymbolic(userPermissions, special, Type::User) + getSymbolic(groupPermissions, special, Type::Group) + getSymbolic(otherPermissions, special, Type::Other));
}

void ChmodCalculatorWidget::updateCheckboxesNumeric()
{
    const int READ_PERMISSION = 4;
    const int WRITE_PERMISSION = 2;
    const int RUN_PERMISSION = 1;
    const QString numeric = ui->numeric->text();
    if(numeric.length() == 3 || numeric.length() == 4)
    {
        int index = 0;
        if(numeric.length() == 4)
        {
            const int special = numeric.at(0).digitValue();
            ui->setuid->setChecked(special & 4);
            ui->setgui->setChecked(special & 2);
            ui->sticky->setChecked(special & 1);
            index = 1;
        }
        else
        {
            ui->setuid->setChecked(false);
            ui->setgui->setChecked(false);
            ui->sticky->setChecked(false);
        }
        const int userPermissions = numeric.at(index++).digitValue();
        ui->readOwner->setChecked(userPermissions & READ_PERMISSION);
        ui->runOwner->setChecked(userPermissions & RUN_PERMISSION);
        ui->writeOwner->setChecked(userPermissions & WRITE_PERMISSION);
        const int groupPermissions = numeric.at(index++).digitValue();
        ui->readGroup->setChecked(groupPermissions & READ_PERMISSION);
        ui->runGroup->setChecked(groupPermissions & RUN_PERMISSION);
        ui->writeGroup->setChecked(groupPermissions & WRITE_PERMISSION);
        const int otherPermissions = numeric.at(index++).digitValue();
        ui->readOther->setChecked(otherPermissions & READ_PERMISSION);
        ui->runOther->setChecked(otherPermissions & RUN_PERMISSION);
        ui->writeOther->setChecked(otherPermissions & WRITE_PERMISSION);
    }
    generate();
}

void ChmodCalculatorWidget::updateCheckBoxesSymbolic()
{
    const QString symbolic = ui->symbolic->text();
    if(symbolic.length() == 9)
    {
        int index = 0;
        ui->readOwner->setChecked(symbolic.at(index++) == 'r');
        ui->writeOwner->setChecked(symbolic.at(index++) == 'w');
        ui->runOwner->setChecked(symbolic.at(index) == 'x' || symbolic.at(index) == 's');
        ui->setuid->setChecked(symbolic.at(index) == 'S' || symbolic.at(index) == 's');
        index++;
        ui->readGroup->setChecked(symbolic.at(index++) == 'r');
        ui->writeGroup->setChecked(symbolic.at(index++) == 'w');
        ui->runGroup->setChecked(symbolic.at(index) == 'x' || symbolic.at(index) == 's');
        ui->setgui->setChecked(symbolic.at(index) == 'S' || symbolic.at(index) == 's');
        index++;
        ui->readOther->setChecked(symbolic.at(index++) == 'r');
        ui->writeOther->setChecked(symbolic.at(index++) == 'w');
        ui->runOther->setChecked(symbolic.at(index) == 'x' || symbolic.at(index) == 't');
        ui->sticky->setChecked(symbolic.at(index) == 'T' || symbolic.at(index) == 't');
    }
    generate();
}
