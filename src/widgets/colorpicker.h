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
    void changedRgb255();
    void changedRgbF();
    void changedRgb64();
    void changedCmyk();
    void changedCmykF();
};

#endif // COLORPICKER_H
