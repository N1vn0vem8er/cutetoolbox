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

private slots:
    void selectColor();
    void selectColorFromScreen();
    void updateColor();
    void copyQColor();
    void pasteQColor();
};

#endif // COLORPICKER_H
