#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class CustomWidget;
}

class ColorPicker : public CustomWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = nullptr);
    ~ColorPicker();

private:
    Ui::CustomWidget *ui;
    bool lock {false};

private slots:
    void selectColor();
    void selectColorFromScreen();
    void updateColor();
    void copyQColor();
    void pasteQColor();
    void pasteCSS();
    void changedRgb255();
    void changedRgbF();
    void changedRgb64();
    void changedCmyk();
    void changedCmykF();
    void changedHsv();
    void changedHsvF();
    void changedHsl();
    void changedHslF();
    void changedHex();
    void changedHexARGB();
};

#endif // COLORPICKER_H
