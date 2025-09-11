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
    connect(ui->cmykK, &QSpinBox::valueChanged, this, &ColorPicker::changedCmyk);
    connect(ui->cmykM, &QSpinBox::valueChanged, this, &ColorPicker::changedCmyk);
    connect(ui->cmykY, &QSpinBox::valueChanged, this, &ColorPicker::changedCmyk);
    connect(ui->cmykC, &QSpinBox::valueChanged, this, &ColorPicker::changedCmyk);
    connect(ui->cmykA, &QSpinBox::valueChanged, this, &ColorPicker::changedCmyk);
    connect(ui->cmykfC, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedCmykF);
    connect(ui->cmykfM, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedCmykF);
    connect(ui->cmykfY, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedCmykF);
    connect(ui->cmykfK, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedCmykF);
    connect(ui->cmykfA, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedCmykF);
    connect(ui->hslH, &QSpinBox::valueChanged, this, &ColorPicker::changedHsl);
    connect(ui->hslS, &QSpinBox::valueChanged, this, &ColorPicker::changedHsl);
    connect(ui->hslL, &QSpinBox::valueChanged, this, &ColorPicker::changedHsl);
    connect(ui->hslA, &QSpinBox::valueChanged, this, &ColorPicker::changedHsl);
    connect(ui->hslfH, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHslF);
    connect(ui->hslfS, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHslF);
    connect(ui->hslfL, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHslF);
    connect(ui->hslfA, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHslF);
    connect(ui->hsvH, &QSpinBox::valueChanged, this, &ColorPicker::changedHsv);
    connect(ui->hsvS, &QSpinBox::valueChanged, this, &ColorPicker::changedHsv);
    connect(ui->hsvV, &QSpinBox::valueChanged, this, &ColorPicker::changedHsv);
    connect(ui->hsvA, &QSpinBox::valueChanged, this, &ColorPicker::changedHsv);
    connect(ui->hsvfH, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHsvF);
    connect(ui->hsvfS, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHsvF);
    connect(ui->hsvfV, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHsvF);
    connect(ui->hsvfA, &QDoubleSpinBox::valueChanged, this, &ColorPicker::changedHsvF);
    connect(ui->hexRGBLineEdit, &QLineEdit::textChanged, this, &ColorPicker::changedHex);
    connect(ui->hexARGBLineEdit, &QLineEdit::textChanged, this, &ColorPicker::changedHexARGB);
    connect(ui->copyQColorButton, &QPushButton::clicked, this, &ColorPicker::copyQColor);
    connect(ui->pasteQColorButton, &QPushButton::clicked, this, &ColorPicker::pasteQColor);
    connect(ui->pasteCSSButton, &QPushButton::clicked, this, &ColorPicker::pasteCSS);
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
    QRegularExpressionMatch match = QRegularExpression(R"(QColor::fromRgbF\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->redf->setValue(match.captured(1).toFloat());
        ui->greenf->setValue(match.captured(2).toFloat());
        ui->bluef->setValue(match.captured(3).toFloat());
        if(match.capturedTexts().length() >= 5)
            ui->alphaf->setValue(match.captured(4).toFloat());
        return;
    }
    match = QRegularExpression(R"(QColor::fromRgb64\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->red64->setValue(match.captured(1).toInt());
        ui->green64->setValue(match.captured(2).toInt());
        ui->blue64->setValue(match.captured(3).toInt());
        if(match.capturedTexts().length() >= 5)
            ui->alpha64->setValue(match.captured(5).toInt());
        return;
    }
    match = QRegularExpression(R"(QColor::fromRgb\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->red255->setValue(match.captured(1).toInt());
        ui->green255->setValue(match.captured(2).toInt());
        ui->blue255->setValue(match.captured(3).toInt());
        if(match.capturedTexts().length() >= 5)
            ui->alpha255->setValue(match.captured(5).toInt());
        return;
    }
    match = QRegularExpression(R"(QColor::fromCmyk\((\d{1,3}), (\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->cmykC->setValue(match.captured(1).toInt());
        ui->cmykM->setValue(match.captured(2).toInt());
        ui->cmykY->setValue(match.captured(3).toInt());
        ui->cmykK->setValue(match.captured(4).toInt());
        if(match.capturedTexts().length() >= 6)
            ui->cmykA->setValue(match.captured(6).toInt());
        return;
    }
    match = QRegularExpression(R"(QColor::fromCmykF\((\d{1,3}), (\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->cmykfC->setValue(match.captured(1).toFloat());
        ui->cmykfM->setValue(match.captured(2).toFloat());
        ui->cmykfY->setValue(match.captured(3).toFloat());
        ui->cmykfK->setValue(match.captured(4).toFloat());
        if(match.capturedTexts().length() >= 6)
            ui->cmykfA->setValue(match.captured(6).toFloat());
        return;
    }
    match = QRegularExpression(R"(QColor::fromHsv\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->hsvH->setValue(match.captured(1).toInt());
        ui->hsvS->setValue(match.captured(2).toInt());
        ui->hsvV->setValue(match.captured(3).toInt());
        if(match.capturedTexts().length() >= 5)
            ui->hsvA->setValue(match.captured(5).toInt());
        return;
    }
    match = QRegularExpression(R"(QColor::fromHsvF\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->hsvfH->setValue(match.captured(1).toFloat());
        ui->hsvfS->setValue(match.captured(2).toFloat());
        ui->hsvfV->setValue(match.captured(3).toFloat());
        if(match.capturedTexts().length() >= 5)
            ui->hsvfA->setValue(match.captured(5).toFloat());
        return;
    }
    match = QRegularExpression(R"(QColor::fromHsl\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->hslH->setValue(match.captured(1).toInt());
        ui->hslS->setValue(match.captured(2).toInt());
        ui->hslL->setValue(match.captured(3).toInt());
        if(match.capturedTexts().length() >= 5)
            ui->hslA->setValue(match.captured(5).toInt());
        return;
    }
    match = QRegularExpression(R"(QColor::fromHslF\((\d{1,3}), (\d{1,3}), (\d{1,3})(, (\d{1,3}))?\))").match(text);
    if(match.hasMatch())
    {
        ui->hslfH->setValue(match.captured(1).toFloat());
        ui->hslfS->setValue(match.captured(2).toFloat());
        ui->hslfL->setValue(match.captured(3).toFloat());
        if(match.capturedTexts().length() >= 5)
            ui->hslfA->setValue(match.captured(5).toFloat());
        return;
    }
};

void ColorPicker::pasteCSS()
{

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
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
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
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedRgb64()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromRgba64(ui->red64->value(), ui->green64->value(), ui->blue64->value(), ui->alpha64->value());
        if(color.isValid())
        {
            ui->red255->setValue(color.red());
            ui->green255->setValue(color.green());
            ui->blue255->setValue(color.blue());
            ui->alpha255->setValue(color.alpha());
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
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
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedCmyk()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromCmyk(ui->cmykC->value(), ui->cmykM->value(), ui->cmykY->value(), ui->cmykK->value(), ui->cmykA->value());
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
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
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
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedCmykF()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromCmykF(ui->cmykfC->value(), ui->cmykfM->value(), ui->cmykfY->value(), ui->cmykfK->value(), ui->cmykfA->value());
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
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
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
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedHsv()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromHsv(ui->hsvH->value(), ui->hsvS->value(), ui->hsvV->value(), ui->hsvA->value());
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
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
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
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedHsvF()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromHsvF(ui->hsvfH->value(), ui->hsvfS->value(), ui->hsvfV->value(), ui->hsvfA->value());
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
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hslH->setValue(color.hslHue());
            ui->hslS->setValue(color.hslSaturation());
            ui->hslL->setValue(color.lightness());
            ui->hslA->setValue(color.alpha());
            ui->hslfH->setValue(color.hslHueF());
            ui->hslfS->setValue(color.hslSaturationF());
            ui->hslfL->setValue(color.lightnessF());
            ui->hslfA->setValue(color.alphaF());
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedHsl()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromHsl(ui->hslH->value(), ui->hslS->value(), ui->hslL->value(), ui->hslA->value());
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
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvfH->setValue(color.hsvHueF());
            ui->hsvfS->setValue(color.hsvSaturationF());
            ui->hsvfV->setValue(color.valueF());
            ui->hsvfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hslfH->setValue(color.hslHueF());
            ui->hslfS->setValue(color.hslSaturationF());
            ui->hslfL->setValue(color.lightnessF());
            ui->hslfA->setValue(color.alphaF());
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedHslF()
{
    if(!lock)
    {
        lock = true;
        const QColor color = QColor::fromHslF(ui->hslfH->value(), ui->hslfS->value(), ui->hslfL->value(), ui->hslfA->value());
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
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hslH->setValue(color.hslHue());
            ui->hslS->setValue(color.hslSaturation());
            ui->hslL->setValue(color.lightness());
            ui->hslA->setValue(color.alpha());
            ui->hsvfH->setValue(color.hsvHueF());
            ui->hsvfS->setValue(color.hsvSaturationF());
            ui->hsvfV->setValue(color.valueF());
            ui->hsvfA->setValue(color.alphaF());
            ui->hexRGBLineEdit->setText(color.name());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedHex()
{
    if(!lock)
    {
        lock = true;
        QString text = ui->hexRGBLineEdit->text();
        if(!text.startsWith("#"))
            text.insert(0, "#");
        const QColor color = QColor::fromString(text);
        if(color.isValid())
        {
            ui->red255->setValue(color.red());
            ui->green255->setValue(color.green());
            ui->blue255->setValue(color.blue());
            ui->alpha255->setValue(color.alpha());
            ui->redf->setValue(color.red());
            ui->greenf->setValue(color.green());
            ui->bluef->setValue(color.blue());
            ui->alphaf->setValue(color.alpha());
            ui->red64->setValue(color.rgba64().red());
            ui->green64->setValue(color.rgba64().green());
            ui->blue64->setValue(color.rgba64().blue());
            ui->alpha64->setValue(color.rgba64().alpha());
            ui->cmykC->setValue(color.cyan());
            ui->cmykK->setValue(color.black());
            ui->cmykM->setValue(color.magenta());
            ui->cmykY->setValue(color.yellow());
            ui->cmykA->setValue(color.alpha());
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hslH->setValue(color.hslHue());
            ui->hslS->setValue(color.hslSaturation());
            ui->hslL->setValue(color.lightness());
            ui->hslA->setValue(color.alpha());
            ui->hsvfH->setValue(color.hsvHueF());
            ui->hsvfS->setValue(color.hsvSaturationF());
            ui->hsvfV->setValue(color.valueF());
            ui->hsvfA->setValue(color.alphaF());
            ui->hexARGBLineEdit->setText(color.name(QColor::HexArgb));
            updateColor();
        }
        lock = false;
    }
}

void ColorPicker::changedHexARGB()
{
    if(!lock)
    {
        lock = true;
        QString text = ui->hexARGBLineEdit->text();
        if(!text.startsWith("#"))
            text.insert(0, "#");
        const QColor color = QColor::fromString(text);
        if(color.isValid())
        {
            ui->red255->setValue(color.red());
            ui->green255->setValue(color.green());
            ui->blue255->setValue(color.blue());
            ui->alpha255->setValue(color.alpha());
            ui->redf->setValue(color.red());
            ui->greenf->setValue(color.green());
            ui->bluef->setValue(color.blue());
            ui->alphaf->setValue(color.alpha());
            ui->red64->setValue(color.rgba64().red());
            ui->green64->setValue(color.rgba64().green());
            ui->blue64->setValue(color.rgba64().blue());
            ui->alpha64->setValue(color.rgba64().alpha());
            ui->cmykC->setValue(color.cyan());
            ui->cmykK->setValue(color.black());
            ui->cmykM->setValue(color.magenta());
            ui->cmykY->setValue(color.yellow());
            ui->cmykA->setValue(color.alpha());
            ui->redf->setValue(color.redF());
            ui->greenf->setValue(color.greenF());
            ui->bluef->setValue(color.blueF());
            ui->alphaf->setValue(color.alphaF());
            ui->cmykfC->setValue(color.cyanF());
            ui->cmykfK->setValue(color.blackF());
            ui->cmykfM->setValue(color.magentaF());
            ui->cmykfY->setValue(color.yellowF());
            ui->cmykfA->setValue(color.alphaF());
            ui->hsvH->setValue(color.hsvHue());
            ui->hsvS->setValue(color.hsvSaturation());
            ui->hsvV->setValue(color.value());
            ui->hsvA->setValue(color.alpha());
            ui->hslH->setValue(color.hslHue());
            ui->hslS->setValue(color.hslSaturation());
            ui->hslL->setValue(color.lightness());
            ui->hslA->setValue(color.alpha());
            ui->hsvfH->setValue(color.hsvHueF());
            ui->hsvfS->setValue(color.hsvSaturationF());
            ui->hsvfV->setValue(color.valueF());
            ui->hsvfA->setValue(color.alphaF());
            ui->hexRGBLineEdit->setText(color.name());
            updateColor();
        }
        lock = false;
    }
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
