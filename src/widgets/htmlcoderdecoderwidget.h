#ifndef HTMLCODERDECODERWIDGET_H
#define HTMLCODERDECODERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class HTMLCoderDecoderWidget;
}

class HTMLCoderDecoderWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit HTMLCoderDecoderWidget(QWidget *parent = nullptr);
    ~HTMLCoderDecoderWidget();

private:
    Ui::HTMLCoderDecoderWidget *ui;
    bool coding {false};

private slots:
    void encode();
    void decode();

public:
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
};

#endif // HTMLCODERDECODERWIDGET_H
