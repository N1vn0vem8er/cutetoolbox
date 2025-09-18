#ifndef COLORPALETTEGENERATORWIDGET_H
#define COLORPALETTEGENERATORWIDGET_H

#include "colorcardwidget.h"
#include "customwidget.h"
#include <QWidget>
#include <qfuturewatcher.h>

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
    QFutureWatcher<QList<QPair<bool, QColor>>> watcher;

private:
    QColor generateRandomColor() const;
    float calculateContrastRatio(const QColor& color1, const QColor& color2);
    void setUiEnabled(bool val);

private slots:
    void generate();
    void colorRemoved(ColorCardWidget* widget);
    void addColor();
    void onContrastCheckPressed();
};

#endif // COLORPALETTEGENERATORWIDGET_H
