#include "colorpicker.h"
#include "src/widgets/ui_colorpicker.h"

#include <QColorDialog>
#include <qvalidator.h>

ColorPicker::ColorPicker(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CustomWidget)
{
    ui->setupUi(this);
    ui->redLineEdit->setValidator(new QIntValidator(0, 255, ui->redLineEdit));
    ui->greenLineEdit->setValidator(new QIntValidator(0, 255, ui->greenLineEdit));
    ui->blueLineEdit->setValidator(new QIntValidator(0, 255, ui->blueLineEdit));
    connect(ui->selectColorButton, &QPushButton::clicked, this, &ColorPicker::selectColor);
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

void ColorPicker::selectColor()
{
    const QColor color = QColorDialog::getColor(QColor::fromRgb(ui->redLineEdit->text().toInt(), ui->greenLineEdit->text().toInt(), ui->blueLineEdit->text().toInt()), this, tr("Pick Color"));
    if(color.isValid())
    {
        ui->redLineEdit->setText(QString::number(color.red()));
        ui->greenLineEdit->setText(QString::number(color.green()));
        ui->blueLineEdit->setText(QString::number(color.blue()));
    }
}
