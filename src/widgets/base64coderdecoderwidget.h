#ifndef BASE64CODERDECODERWIDGET_H
#define BASE64CODERDECODERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class Base64CoderDecoderWidget;
}

class Base64CoderDecoderWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit Base64CoderDecoderWidget(QWidget *parent = nullptr);
    ~Base64CoderDecoderWidget();

private:
    Ui::Base64CoderDecoderWidget *ui;
    bool coding {false};

private slots:
    void encode();
    void decode();
};

#endif // BASE64CODERDECODERWIDGET_H
