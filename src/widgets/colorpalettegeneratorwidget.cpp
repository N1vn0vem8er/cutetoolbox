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
    connect(ui->checkContrastCheckBox, &QCheckBox::clicked, this, &ColorPaletteGeneratorWidget::onContrastCheckPressed);
    ui->contrastSpinBox->setVisible(false);
    ui->attemptsLabel->setVisible(false);
    ui->attemptsSpinBox->setVisible(false);
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

float ColorPaletteGeneratorWidget::calculateContrastRatio(const QColor &color1, const QColor &color2)
{
    QColor foreground;
    float alpha = color1.alphaF();
    foreground.setRedF(color1.redF() * alpha + color2.redF() * (1 - alpha));
    foreground.setGreenF(color1.greenF() * alpha + color2.greenF() * (1 - alpha));
    foreground.setBlueF(color1.blueF() * alpha + color2.blueF() * (1 - alpha));
    foreground.setAlpha(255);
    auto calculateLuminance = [&](const QColor& color){
        float rLinear = color.redF();
        float gLinear = color.greenF();
        float bLinear = color.blueF();
        auto srgbToLinear = [](float c) {
            if (c <= 0.03928) {
                return c / 12.92;
            } else {
                return std::pow((c + 0.055) / 1.055, 2.4);
            }
        };
        float rSrgb = srgbToLinear(rLinear);
        float gSrgb = srgbToLinear(gLinear);
        float bSrgb = srgbToLinear(bLinear);

        return 0.2126 * rSrgb + 0.7152 * gSrgb + 0.0722 * bSrgb;
    };
    float l1 = calculateLuminance(foreground);
    float l2 = calculateLuminance(color2);
    if(l2 > l1)
        std::swap(l1, l2);
    return (l1 + 0.05) / (l2 + 0.05);
}

void ColorPaletteGeneratorWidget::generate()
{
    if(!ui->checkContrastCheckBox->isChecked())
    {
        for(ColorCardWidget* widget : std::as_const(colorWidgets))
        {
            widget->setColor(generateRandomColor());
        }
    }
    else
    {
        float minContrastRatio = ui->contrastSpinBox->value();
        int maxAttempts = ui->attemptsSpinBox->value();
        for(int i = 0; i < colorWidgets.length(); i++)
        {
            bool suitableColorFound = false;
            int attempts = 0;
            QColor newColor;
            while(!suitableColorFound && attempts < maxAttempts)
            {
                newColor = generateRandomColor();
                bool hasSufficientContrast = true;
                for(int j = 0; j < colorWidgets.length(); ++j)
                {
                    if(i == j) continue;
                    if(calculateContrastRatio(newColor, colorWidgets[j]->getColor()) < minContrastRatio)
                    {
                        hasSufficientContrast = false;
                        break;
                    }
                }
                if(hasSufficientContrast)
                {
                    suitableColorFound = true;
                }
                attempts++;
            }
            if(!suitableColorFound)
                colorWidgets[i]->setBadContrast(true);
            else
            {
                if(!colorWidgets[i]->isLocked())
                    colorWidgets[i]->setColor(newColor);
                colorWidgets[i]->setBadContrast(false);
            }
        }
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
    if(ui->checkContrastCheckBox->isChecked())
    {
        bool suitableColorFound = false;
        int attempts = 0;
        while(!suitableColorFound && attempts < ui->attemptsSpinBox->value())
        {
            QColor newColor = generateRandomColor();
            bool hasSufficientContrast = true;
            for(int j = 0; j < colorWidgets.length(); ++j)
            {
                if(calculateContrastRatio(newColor, colorWidgets[j]->getColor()) < ui->contrastSpinBox->value())
                {
                    hasSufficientContrast = false;
                    break;
                }
            }
            if(hasSufficientContrast)
            {
                widget->setColor(newColor);
                suitableColorFound = true;
            }
            attempts++;
        }
        if(!suitableColorFound)
            widget->setBadContrast(true);
        else
            widget->setBadContrast(false);
    }
    else
        widget->setColor(generateRandomColor());
    colorWidgets.append(widget);
    ui->horizontalWidget->layout()->addWidget(widget);
}

void ColorPaletteGeneratorWidget::onContrastCheckPressed()
{
    if(ui->checkContrastCheckBox->isChecked())
    {
        ui->contrastSpinBox->setVisible(true);
        ui->attemptsLabel->setVisible(true);
        ui->attemptsSpinBox->setVisible(true);
    }
    else
    {
        ui->contrastSpinBox->setVisible(false);
        ui->attemptsLabel->setVisible(false);
        ui->attemptsSpinBox->setVisible(false);
        for(ColorCardWidget* widget : std::as_const(colorWidgets))
        {
            widget->setBadContrast(false);
        }
    }
    generate();
}
