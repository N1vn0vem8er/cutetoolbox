#include "colorpalettegeneratorwidget.h"
#include "colorcardwidget.h"
#include "src/widgets/ui_colorpalettegeneratorwidget.h"
#include <QColorDialog>
#include <random>

ColorPaletteGeneratorWidget::ColorPaletteGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ColorPaletteGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("Color Palette Generator"));
    connect(ui->generateColorsButton, &QPushButton::clicked, this, &ColorPaletteGeneratorWidget::generate);
    connect(ui->addColorButton, &QPushButton::clicked, this, &ColorPaletteGeneratorWidget::addColor);
    for(int i=0; i<5; i++)
    {
        addColor();
    }
}

ColorPaletteGeneratorWidget::~ColorPaletteGeneratorWidget()
{
    delete ui;
}

QColor ColorPaletteGeneratorWidget::generateRandomColor() const
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(0, 255);
    return QColor::fromRgb(random(generator), random(generator), random(generator));
}

void ColorPaletteGeneratorWidget::generate()
{
    for(ColorCardWidget* widget : std::as_const(colorWidgets))
    {
        widget->setColor(generateRandomColor());
    }
}

void ColorPaletteGeneratorWidget::colorRemoved(ColorCardWidget *widget)
{
    ui->horizontalWidget->layout()->removeWidget(widget);
    colorWidgets.removeAll(widget);
    widget->deleteLater();
}

void ColorPaletteGeneratorWidget::addColor()
{
    ColorCardWidget* widget = new ColorCardWidget(ui->horizontalWidget);
    connect(widget, &ColorCardWidget::removeColor, this, &ColorPaletteGeneratorWidget::colorRemoved);
    widget->setColor(generateRandomColor());
    colorWidgets.append(widget);
    ui->horizontalWidget->layout()->addWidget(widget);
}
