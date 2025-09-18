#ifndef COLORPALETTEGENERATORWIDGET_H
#define COLORPALETTEGENERATORWIDGET_H

#include "colorcardwidget.h"
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
    QList<ColorCardWidget*> colorWidgets;

private:
    QColor generateRandomColor() const;

private slots:
    void generate();
    void colorRemoved(ColorCardWidget* widget);
    void addColor();
};

#endif // COLORPALETTEGENERATORWIDGET_H
