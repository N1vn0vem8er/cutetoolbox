#ifndef COLORPALETTEGENERATORWIDGET_H
#define COLORPALETTEGENERATORWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ColorPaletteGeneratorWidget;
}

class ColorPaletteGeneratorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ColorPaletteGeneratorWidget(QWidget *parent = nullptr);
    ~ColorPaletteGeneratorWidget();

private:
    Ui::ColorPaletteGeneratorWidget *ui;
    QList<QColor> colors {QColor(), QColor(), QColor(), QColor(), QColor(), QColor(), QColor(), QColor(), QColor(), QColor()};
    QList<bool> lockedColors {false, false, false, false, false, false, false, false, false, false};
    int colorsCount {0};

private:
    void setColorsVisible(int count);
    void setColorsInvisible();
    QColor generateRandomColor() const;
    void updateColors();

private slots:
    void generate();
};

#endif // COLORPALETTEGENERATORWIDGET_H
