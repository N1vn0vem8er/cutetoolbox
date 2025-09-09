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
    connect(ui->red255, &QSpinBox::valueChanged, this, &ColorPicker::updateColor);
    connect(ui->green255, &QSpinBox::valueChanged, this, &ColorPicker::updateColor);
    connect(ui->blue255, &QSpinBox::valueChanged, this, &ColorPicker::updateColor);
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
    palette.setColor(QPalette::Window, QColor::fromRgb(ui->red255->value(), ui->green255->value(), ui->blue255->value()));
    ui->colorWidget->setPalette(palette);
}

void ColorPicker::copyQColor()
{
    QGuiApplication::clipboard()->setText(QString("QColor::fromRgb(%1, %2, %3)").arg(ui->red255->value()).arg(ui->green255->value()).arg(ui->blue255->value()));
}

void ColorPicker::pasteQColor()
{
    QString text = QGuiApplication::clipboard()->text();
    if(text.contains("QColor::fromRgb"))
    {
        QRegularExpressionMatch match = QRegularExpression(R"(QColor::fromRgb\((\d{1,3}), (\d{1,3}), (\d{1,3})\))").globalMatch(text).next();
        if(match.hasMatch())
        {
            ui->red255->setValue(match.captured(1).toInt());
            ui->green255->setValue(match.captured(2).toInt());
            ui->blue255->setValue(match.captured(3).toInt());
        }
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
