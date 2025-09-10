#include "colorpicker.h"
#include "src/widgets/ui_colorpicker.h"
#include <QColorDialog>
#include <QMouseEvent>
#include <qvalidator.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QClipboard>

ColorPicker::ColorPicker(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CustomWidget)
{
    ui->setupUi(this);
    connect(ui->selectColorButton, &QPushButton::clicked, this, &ColorPicker::selectColor);
    connect(ui->selectFromScreenButton, &QPushButton::clicked, this, &ColorPicker::selectColorFromScreen);
    connect(ui->red255, &QSpinBox::valueChanged, this, &ColorPicker::changedRgb255);
    connect(ui->green255, &QSpinBox::valueChanged, this, &ColorPicker::changedRgb255);
    connect(ui->blue255, &QSpinBox::valueChanged, this, &ColorPicker::changedRgb255);
    connect(ui->alpha255, &QSpinBox::valueChanged, this, &ColorPicker::changedRgb255);
    connect(ui->redf, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedRgbF);
    connect(ui->greenf, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedRgbF);
    connect(ui->bluef, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedRgbF);
    connect(ui->alphaf, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedRgbF);
    connect(ui->copyQColorButton, &QPushButton::clicked, this, &ColorPicker::copyQColor);
    connect(ui->pasteQColorButton, &QPushButton::clicked, this, &ColorPicker::pasteQColor);
    ui->colorWidget->setAutoFillBackground(true);
    updateColor();
}

ColorPicker::~ColorPicker()
{
    delete ui;
}

void ColorPicker::updateColor()
{
    QPalette palette = ui->colorWidget->palette();
    palette.setColor(QPalette::Window, QColor::fromRgb(ui->red255->value(), ui->green255->value(), ui->blue255->value(), ui->alpha255->value()));
    ui->colorWidget->setPalette(palette);
}

void ColorPicker::copyQColor()
{
    QGuiApplication::clipboard()->setText(QString("QColor::fromRgb(%1, %2, %3)").arg(ui->red255->value()).arg(ui->green255->value()).arg(ui->blue255->value()));
}

void ColorPicker::pasteQColor()
{
    QString text = QGuiApplication::clipboard()->text();
    if(text.contains("QColor::fromRgbF"))
    {

    }
    else if(text.contains("QColor::fromRgb64"))
    {

    }
    else if(text.contains("QColor::fromRgb"))
    {
        QRegularExpressionMatch match = QRegularExpression(R"(QColor::fromRgb\((\d{1,3}), (\d{1,3}), (\d{1,3})\))").globalMatch(text).next();
        if(match.hasMatch())
        {
            ui->red255->setValue(match.captured(1).toInt());
            ui->green255->setValue(match.captured(2).toInt());
            ui->blue255->setValue(match.captured(3).toInt());
        }
    }
    else if(text.contains("QColor::fromCmyk"))
    {

    }
}

void ColorPicker::changedRgb255()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromRgb(ui->red255->value(), ui->green255->value(), ui->blue255->value(), ui->alpha255->value());
        if(color.isValid())
        {
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->red64->setValue(color.rgba64().red());
            ui->green64->setValue(color.rgba64().green());
            ui->blue64->setValue(color.rgba64().blue());
            ui->alpha64->setValue(color.rgba64().alpha());
            ui->cmykC->setValue(color.cyan());
            ui->cmykK->setValue(color.black());
            ui->cmykM->setValue(color.magenta());
            ui->cmykY->setValue(color.yellow());
            ui->cmykA->setValue(color.alpha());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hsvfH->setValue(color.hsvHueF());
            ui->hsvfS->setValue(color.hsvSaturationF());
            ui->hsvfV->setValue(color.valueF());
            ui->hsvfA->setValue(color.alphaF());
            ui->hslH->setValue(color.hslHue());
            ui->hslS->setValue(color.hslSaturation());
            ui->hslL->setValue(color.lightness());
            ui->hslA->setValue(color.alpha());
            ui->hslfH->setValue(color.hslHueF());
            ui->hslfS->setValue(color.hslSaturationF());
            ui->hslfL->setValue(color.lightnessF());
            ui->hslfA->setValue(color.alphaF());
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedRgbF()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromRgbF(ui->redf->value(), ui->greenf->value(), ui->bluef->value(), ui->alphaf->value());
        if(color.isValid())
        {
            ui->red255->setValue(color.red());
            ui->green255->setValue(color.green());
            ui->blue255->setValue(color.blue());
            ui->alpha255->setValue(color.alpha());
            ui->red64->setValue(color.rgba64().red());
            ui->green64->setValue(color.rgba64().green());
            ui->blue64->setValue(color.rgba64().blue());
            ui->alpha64->setValue(color.rgba64().alpha());
            ui->cmykC->setValue(color.cyan());
            ui->cmykK->setValue(color.black());
            ui->cmykM->setValue(color.magenta());
            ui->cmykY->setValue(color.yellow());
            ui->cmykA->setValue(color.alpha());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hsvfH->setValue(color.hsvHueF());
            ui->hsvfS->setValue(color.hsvSaturationF());
            ui->hsvfV->setValue(color.valueF());
            ui->hsvfA->setValue(color.alphaF());
            ui->hslH->setValue(color.hslHue());
            ui->hslS->setValue(color.hslSaturation());
            ui->hslL->setValue(color.lightness());
            ui->hslA->setValue(color.alpha());
            ui->hslfH->setValue(color.hslHueF());
            ui->hslfS->setValue(color.hslSaturationF());
            ui->hslfL->setValue(color.lightnessF());
            ui->hslfA->setValue(color.alphaF());
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedRgb64()
{

}

void ColorPicker::changedCmyk()
{

}

void ColorPicker::changedCmykF()
{

}

void ColorPicker::selectColor()
{
    const QColor color = QColorDialog::getColor(QColor::fromRgb(ui->red255->text().toInt(), ui->green255->text().toInt(), ui->blue255->text().toInt()), this, tr("Pick Color"));
    if(color.isValid())
    {
        ui->red255->setValue(color.red());
        ui->green255->setValue(color.green());
        ui->blue255->setValue(color.blue());
    }
}

void ColorPicker::selectColorFromScreen()
{

}
