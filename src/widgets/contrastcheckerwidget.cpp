#include "contrastcheckerwidget.h"
#include "src/widgets/ui_contrastcheckerwidget.h"

#include <QColorDialog>

ContrastCheckerWidget::ContrastCheckerWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ContrastCheckerWidget)
{
    ui->setupUi(this);
    connect(ui->selectBackgroundColorButton, &QPushButton::clicked, this, &ContrastCheckerWidget::selectBackgroundColor);
    connect(ui->selectTextColorButton, &QPushButton::clicked, this, &ContrastCheckerWidget::selectTextColor);
    ui->textColorWidget->setAutoFillBackground(true);
    ui->backgroundColorWidget->setAutoFillBackground(true);
    QPalette palette = ui->textColorWidget->palette();
    palette.setColor(QPalette::Window, textColor);
    ui->textColorWidget->setPalette(palette);
    palette = ui->backgroundColorWidget->palette();
    palette.setColor(QPalette::Window, backgroundColor);
    ui->backgroundColorWidget->setPalette(palette);
    updateExample();
}

ContrastCheckerWidget::~ContrastCheckerWidget()
{
    delete ui;
}

void ContrastCheckerWidget::updateExample()
{
    QString html = R"(
    <html>
        <body style="background-color: %1">
        <center><h1 style="color: %2">Lorem ipsum dolor sit amet</h1>
        <p style="color: %2">aliqua duis tempor dolor consectetur eiusmod cillum occaecat ex in in ex enim mollit anim dolor magna.</p>
        </body>
    </html>
    )";
    ui->example->setText(html.arg(backgroundColor.name(QColor::HexArgb), textColor.name(QColor::HexArgb)));
    calculateContrastRatio();
}

void ContrastCheckerWidget::calculateContrastRatio()
{
    QColor foreground;
    float alpha = textColor.alphaF();
    foreground.setRedF(textColor.redF() * alpha + backgroundColor.redF() * (1 - alpha));
    foreground.setGreenF(textColor.greenF() * alpha + backgroundColor.greenF() * (1 - alpha));
    foreground.setBlueF(textColor.blueF() * alpha + backgroundColor.blueF() * (1 - alpha));
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
    float l2 = calculateLuminance(backgroundColor);
    if(l2 > l1)
        std::swap(l1, l2);
    float contrast = (l1 + 0.05) / (l2 + 0.05);
    if(contrast > 4.5)
        ui->normalTextAA->setText("Normal Text AA: Pass");
    else
        ui->normalTextAA->setText("Normal Text AA: Fail");
    if(contrast > 3)
        ui->largeTextAA->setText("Large Text AA: Pass");
    else
        ui->largeTextAA->setText("Large Text AA: Fail");
    if(contrast > 7)
        ui->normallTextAAA->setText("Normal Text AAA: Pass");
    else
        ui->normallTextAAA->setText("Normal Text AAA: Fail");
    if(contrast > 4)
        ui->largeTextAAA->setText("Large Text AAA: Pass");
    else
        ui->largeTextAAA->setText("Large Text AAA: Fail");
    ui->contrast->setText(QString("Contrast: %1:1").arg(contrast));
}

void ContrastCheckerWidget::selectTextColor()
{
    textColor = QColorDialog::getColor(textColor, this, tr("Select Color"));
    QPalette palette = ui->textColorWidget->palette();
    palette.setColor(QPalette::Window, textColor);
    ui->textColorWidget->setPalette(palette);
    updateExample();
}

void ContrastCheckerWidget::selectBackgroundColor()
{
    backgroundColor = QColorDialog::getColor(backgroundColor, this, tr("Select Color"));
    QPalette palette = ui->backgroundColorWidget->palette();
    palette.setColor(QPalette::Window, backgroundColor);
    ui->backgroundColorWidget->setPalette(palette);
    updateExample();
}
