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
};

#endif // COLORPALETTEGENERATORWIDGET_H
