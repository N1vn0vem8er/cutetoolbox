#ifndef HTMLCODERDECODERWIDGET_H
#define HTMLCODERDECODERWIDGET_H

#include <QWidget>

namespace Ui {
class HTMLCoderDecoderWidget;
}

class HTMLCoderDecoderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HTMLCoderDecoderWidget(QWidget *parent = nullptr);
    ~HTMLCoderDecoderWidget();

private:
    Ui::HTMLCoderDecoderWidget *ui;
};

#endif // HTMLCODERDECODERWIDGET_H
