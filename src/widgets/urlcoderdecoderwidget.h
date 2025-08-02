#ifndef URLCODERDECODERWIDGET_H
#define URLCODERDECODERWIDGET_H

#include <QWidget>

namespace Ui {
class UrlCoderDecoderWidget;
}

class UrlCoderDecoderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UrlCoderDecoderWidget(QWidget *parent = nullptr);
    ~UrlCoderDecoderWidget();

private:
    Ui::UrlCoderDecoderWidget *ui;
};

#endif // URLCODERDECODERWIDGET_H
