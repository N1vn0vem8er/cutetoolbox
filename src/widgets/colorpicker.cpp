#include "colorpicker.h"
#include "src/widgets/ui_colorpicker.h"
#include <QColorDialog>
#include <QMouseEvent>
#include <qvalidator.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

ColorPicker::ColorPicker(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CustomWidget)
{
    ui->setupUi(this);
    ui->redLineEdit->setValidator(new QIntValidator(0, 255, ui->redLineEdit));
    ui->greenLineEdit->setValidator(new QIntValidator(0, 255, ui->greenLineEdit));
    ui->blueLineEdit->setValidator(new QIntValidator(0, 255, ui->blueLineEdit));
    connect(ui->selectColorButton, &QPushButton::clicked, this, &ColorPicker::selectColor);
    connect(ui->selectFromScreenButton, &QPushButton::clicked, this, &ColorPicker::selectColorFromScreen);
    ui->colorWidget->setAutoFillBackground(true);
    QPalette palette = ui->colorWidget->palette();
    palette.setColor(QPalette::Window, QColor::fromRgb(ui->redLineEdit->text().toInt(), ui->greenLineEdit->text().toInt(), ui->blueLineEdit->text().toInt()));
    ui->colorWidget->setPalette(palette);
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
        QPalette palette = ui->colorWidget->palette();
        palette.setColor(QPalette::Window, QColor::fromRgb(ui->redLineEdit->text().toInt(), ui->greenLineEdit->text().toInt(), ui->blueLineEdit->text().toInt()));
        ui->colorWidget->setPalette(palette);
    }
}

void ColorPicker::selectColorFromScreen()
{

}
