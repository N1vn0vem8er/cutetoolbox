#include "colorpalettegeneratorwidget.h"
#include "colorcardwidget.h"
#include "config.h"
#include "src/widgets/ui_colorpalettegeneratorwidget.h"
#include <QColorDialog>
#include <QSettings>
#include <qtconcurrentrun.h>
#include <random>

ColorPaletteGeneratorWidget::ColorPaletteGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ColorPaletteGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("Color Palette Generator"));
    QSettings settings(Config::settingsName);
    int colorCount = settings.value("colorpalettegenerator.color.count", 5).toInt();
    ui->checkContrastCheckBox->setChecked(settings.value("colorpalettegenerator.checkContrast", false).toBool());
    ui->contrastSpinBox->setValue(settings.value("colorpalettegenerator.contrast", 0.0).toFloat());
    ui->attemptsSpinBox->setValue(settings.value("colorpalettegenerator.attempts", 1000).toInt());
    connect(ui->generateColorsButton, &QPushButton::clicked, this, &ColorPaletteGeneratorWidget::generate);
    connect(ui->addColorButton, &QPushButton::clicked, this, &ColorPaletteGeneratorWidget::addColor);
    connect(ui->checkContrastCheckBox, &QCheckBox::clicked, this, &ColorPaletteGeneratorWidget::onContrastCheckPressed);
    connect(&watcher, &QFutureWatcher<QList<QPair<bool, QColor>>>::started, this, [&]{setUiEnabled(false);});
    connect(&watcher, &QFutureWatcher<QList<QPair<bool, QColor>>>::finished, this, [&]{
        QList<QPair<bool, QColor>> results = watcher.result();
        for(int i = 0; i<results.length(); i++)
        {
            colorWidgets[i]->setColor(results[i].second);
            colorWidgets[i]->setBadContrast(results[i].first);
        }
        setUiEnabled(true);
    });
    ui->contrastSpinBox->setVisible(false);
    ui->attemptsLabel->setVisible(false);
    ui->attemptsSpinBox->setVisible(false);
    connect(&addColorWatcher, &QFutureWatcher<QPair<QColor, bool>>::started, this, [&]{setUiEnabled(false);});
    connect(&addColorWatcher, &QFutureWatcher<QPair<QColor, bool>>::finished, this, [this]{
        QPair<QColor, bool> color = addColorWatcher.result();
        ColorCardWidget* widget = new ColorCardWidget(ui->horizontalWidget);
        connect(widget, &ColorCardWidget::removeColor, this, &ColorPaletteGeneratorWidget::colorRemoved);
        widget->setColor(color.first);
        widget->setBadContrast(color.second);
        colorWidgets.append(widget);
        ui->horizontalWidget->layout()->addWidget(widget);
        setUiEnabled(true);
    });
    connect(&initColorsWatcher, &QFutureWatcher<QList<QPair<QColor, bool>>>::started, this, [&]{setUiEnabled(false);});
    connect(&initColorsWatcher, &QFutureWatcher<QList<QPair<QColor, bool>>>::finished, this, [this]{
        const QList<QPair<QColor, bool>> results = initColorsWatcher.result();
        for(const auto& color : results)
        {
            ColorCardWidget* widget = new ColorCardWidget(ui->horizontalWidget);
            connect(widget, &ColorCardWidget::removeColor, this, &ColorPaletteGeneratorWidget::colorRemoved);
            widget->setColor(color.first);
            widget->setBadContrast(color.second);
            colorWidgets.append(widget);
            ui->horizontalWidget->layout()->addWidget(widget);
        }
        setUiEnabled(true);
    });
    int attemptsSpinBox = ui->attemptsSpinBox->value();
    bool checkContrast = ui->checkContrastCheckBox->isChecked();
    int contrast= ui->contrastSpinBox->value();
    QFuture<QList<QPair<QColor, bool>>> future = QtConcurrent::run([this, attemptsSpinBox, checkContrast, contrast, colorCount]{
        QList<QPair<QColor, bool>> colors;
        for(int i=0; i<colorCount; i++)
        {
            QPair<QColor, bool> color;
            if(checkContrast)
            {
                bool suitableColorFound = false;
                int attempts = 0;
                while(!suitableColorFound && attempts < attemptsSpinBox)
                {
                    QColor newColor = generateRandomColor();
                    bool hasSufficientContrast = true;
                    for(int j = 0; j < colors.length(); ++j)
                    {
                        if(calculateContrastRatio(newColor, colors[j].first) < contrast)
                        {
                            hasSufficientContrast = false;
                            break;
                        }
                    }
                    if(hasSufficientContrast)
                    {
                        color.first = newColor;
                        suitableColorFound = true;
                    }
                    attempts++;
                }
                if(!suitableColorFound)
                    color.second = true;
                else
                    color.second = false;
            }
            else
                color.first = generateRandomColor();
            colors.append(color);
        }
        return colors;
    });
    initColorsWatcher.setFuture(future);
}

ColorPaletteGeneratorWidget::~ColorPaletteGeneratorWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("colorpalettegenerator.color.count", colorWidgets.length());
    settings.setValue("colorpalettegenerator.checkContrast", ui->checkContrastCheckBox->isChecked());
    settings.setValue("colorpalettegenerator.contrast", ui->contrastSpinBox->value());
    settings.setValue("colorpalettegenerator.attempts", ui->attemptsSpinBox->value());
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

void ColorPaletteGeneratorWidget::setUiEnabled(bool val)
{
    ui->generateColorsButton->setEnabled(val);
    ui->addColorButton->setEnabled(val);
    ui->checkContrastCheckBox->setEnabled(val);
    ui->attemptsSpinBox->setEnabled(val);
    ui->contrastSpinBox->setEnabled(val);
    for(ColorCardWidget* i : std::as_const(colorWidgets))
    {
        i->setEnabled(val);
    }
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
        QList<QPair<bool, QColor>> colors;
        for(ColorCardWidget* widget : std::as_const(colorWidgets))
        {
            colors.append(QPair(widget->isLocked(), widget->getColor()));
        }
        QFuture<QList<QPair<bool, QColor>>> future = QtConcurrent::run([minContrastRatio, maxAttempts, colors, this]{
            QList<QPair<bool, QColor>> ret = colors;
            for(int i = 0; i < ret.length(); i++)
            {
                bool suitableColorFound = false;
                int attempts = 0;
                QColor newColor;
                while(!suitableColorFound && attempts < maxAttempts)
                {
                    newColor = generateRandomColor();
                    bool hasSufficientContrast = true;
                    for(int j = 0; j < ret.length(); j++)
                    {
                        if(i == j) continue;
                        if(calculateContrastRatio(newColor, ret[j].second) < minContrastRatio)
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
                ret[i].second = newColor;
                ret[i].first = !suitableColorFound;
            }
            return ret;
        });
        watcher.setFuture(future);
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
    if(ui->checkContrastCheckBox->isChecked())
    {
        const auto colorWidgets = this->colorWidgets;
        QFuture<QPair<QColor, bool>> future = QtConcurrent::run([this, colorWidgets]
        {
            QPair<QColor, bool> color;
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
                    color.first = newColor;
                    suitableColorFound = true;
                }
                attempts++;
            }
            if(!suitableColorFound)
                color.second = true;
            else
                color.second = false;
            return color;
        });
        addColorWatcher.setFuture(future);
    }
    else
    {
        ColorCardWidget* widget = new ColorCardWidget(ui->horizontalWidget);
        connect(widget, &ColorCardWidget::removeColor, this, &ColorPaletteGeneratorWidget::colorRemoved);
        widget->setColor(generateRandomColor());
        colorWidgets.append(widget);
        ui->horizontalWidget->layout()->addWidget(widget);
    }
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
