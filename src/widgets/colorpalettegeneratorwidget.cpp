#include "colorpalettegeneratorwidget.h"
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
    connect(ui->lockColor1Button, &QPushButton::clicked, this, [&]{lockedColors[0] = !lockedColors[0]; ui->lockColor1Button->setText(lockedColors[0] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor2Button, &QPushButton::clicked, this, [&]{lockedColors[1] = !lockedColors[1]; ui->lockColor2Button->setText(lockedColors[1] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor3Button, &QPushButton::clicked, this, [&]{lockedColors[2] = !lockedColors[2]; ui->lockColor3Button->setText(lockedColors[2] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor4Button, &QPushButton::clicked, this, [&]{lockedColors[3] = !lockedColors[3]; ui->lockColor4Button->setText(lockedColors[3] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor5Button, &QPushButton::clicked, this, [&]{lockedColors[4] = !lockedColors[4]; ui->lockColor5Button->setText(lockedColors[4] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor6Button, &QPushButton::clicked, this, [&]{lockedColors[5] = !lockedColors[5]; ui->lockColor6Button->setText(lockedColors[5] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor7Button, &QPushButton::clicked, this, [&]{lockedColors[6] = !lockedColors[6]; ui->lockColor7Button->setText(lockedColors[6] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor8Button, &QPushButton::clicked, this, [&]{lockedColors[7] = !lockedColors[7]; ui->lockColor8Button->setText(lockedColors[7] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor9Button, &QPushButton::clicked, this, [&]{lockedColors[8] = !lockedColors[8]; ui->lockColor9Button->setText(lockedColors[8] ? tr("Unlock") : tr("Lock"));});
    connect(ui->lockColor10Button, &QPushButton::clicked, this, [&]{lockedColors[9] = !lockedColors[9]; ui->lockColor10Button->setText(lockedColors[9] ? tr("Unlock") : tr("Lock"));});
    connect(ui->selectColor1Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[0], this, tr("Select Color")); if(color.isValid()) colors[0] = color; updateColors();});
    connect(ui->selectColor2Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[1], this, tr("Select Color")); if(color.isValid()) colors[1] = color; updateColors();});
    connect(ui->selectColor3Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[2], this, tr("Select Color")); if(color.isValid()) colors[2] = color; updateColors();});
    connect(ui->selectColor4Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[3], this, tr("Select Color")); if(color.isValid()) colors[3] = color; updateColors();});
    connect(ui->selectColor5Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[4], this, tr("Select Color")); if(color.isValid()) colors[4] = color; updateColors();});
    connect(ui->selectColor6Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[5], this, tr("Select Color")); if(color.isValid()) colors[5] = color; updateColors();});
    connect(ui->selectColor7Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[6], this, tr("Select Color")); if(color.isValid()) colors[6] = color; updateColors();});
    connect(ui->selectColor8Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[7], this, tr("Select Color")); if(color.isValid()) colors[7] = color; updateColors();});
    connect(ui->selectColor9Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[8], this, tr("Select Color")); if(color.isValid()) colors[8] = color; updateColors();});
    connect(ui->selectColor10Button, &QPushButton::clicked, this, [&]{const QColor color = QColorDialog::getColor(colors[9], this, tr("Select Color")); if(color.isValid()) colors[9] = color; updateColors();});
    generate();
    setColorsInvisible();
    setColorsVisible(5);
}

ColorPaletteGeneratorWidget::~ColorPaletteGeneratorWidget()
{
    delete ui;
}

void ColorPaletteGeneratorWidget::setColorsVisible(int count)
{
    switch(count)
    {
    case 10:
        ui->color10Widget->setVisible(true);
        colorsCount++;
    case 9:
        ui->color9Widget->setVisible(true);
        colorsCount++;
    case 8:
        ui->color8Widget->setVisible(true);
        colorsCount++;
    case 7:
        ui->color7Widget->setVisible(true);
        colorsCount++;
    case 6:
        ui->color6Widget->setVisible(true);
        colorsCount++;
    case 5:
        ui->color5Widget->setVisible(true);
        colorsCount++;
    case 4:
        ui->color4Widget->setVisible(true);
        colorsCount++;
    case 3:
        ui->color3Widget->setVisible(true);
        colorsCount++;
    case 2:
        ui->color2Widget->setVisible(true);
        colorsCount++;
    case 1:
        ui->color1Widget->setVisible(true);
        colorsCount++;
    }
}

void ColorPaletteGeneratorWidget::setColorsInvisible()
{
    ui->color10Widget->setVisible(false);
    ui->color9Widget->setVisible(false);
    ui->color8Widget->setVisible(false);
    ui->color7Widget->setVisible(false);
    ui->color6Widget->setVisible(false);
    ui->color5Widget->setVisible(false);
    ui->color4Widget->setVisible(false);
    ui->color3Widget->setVisible(false);
    ui->color2Widget->setVisible(false);
    ui->color1Widget->setVisible(false);
    colorsCount = 0;
}

QColor ColorPaletteGeneratorWidget::generateRandomColor() const
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(0, 255);
    return QColor::fromRgb(random(generator), random(generator), random(generator));
}

void ColorPaletteGeneratorWidget::updateColors()
{
    ui->color10Widget->setStyleSheet(QString("background-color: %1").arg(colors[9].name(QColor::HexArgb)));
    ui->color9Widget->setStyleSheet(QString("background-color: %1").arg(colors[8].name(QColor::HexArgb)));
    ui->color8Widget->setStyleSheet(QString("background-color: %1").arg(colors[7].name(QColor::HexArgb)));
    ui->color7Widget->setStyleSheet(QString("background-color: %1").arg(colors[6].name(QColor::HexArgb)));
    ui->color6Widget->setStyleSheet(QString("background-color: %1").arg(colors[5].name(QColor::HexArgb)));
    ui->color5Widget->setStyleSheet(QString("background-color: %1").arg(colors[4].name(QColor::HexArgb)));
    ui->color4Widget->setStyleSheet(QString("background-color: %1").arg(colors[3].name(QColor::HexArgb)));
    ui->color3Widget->setStyleSheet(QString("background-color: %1").arg(colors[2].name(QColor::HexArgb)));
    ui->color2Widget->setStyleSheet(QString("background-color: %1").arg(colors[1].name(QColor::HexArgb)));
    ui->color1Widget->setStyleSheet(QString("background-color: %1").arg(colors[0].name(QColor::HexArgb)));
}

void ColorPaletteGeneratorWidget::generate()
{
    for(int i = 0; i<colorsCount && i<colors.length(); i++)
    {
        if(!lockedColors[i])
            colors[i] = generateRandomColor();
    }
    updateColors();
}
