#ifndef NUMBERBASESWIDGET_H
#define NUMBERBASESWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class NumberBasesWidget;
}

class NumberBasesWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit NumberBasesWidget(QWidget *parent = nullptr);
    ~NumberBasesWidget();
    bool canBasicEdit() const override;

private:
    Ui::NumberBasesWidget *ui;

private slots:
    void fromBin();
    void fromDec();
    void fromOct();
    void fromHex();
    void fromASCII();
    void fromUTF8();
};

#endif // NUMBERBASESWIDGET_H
