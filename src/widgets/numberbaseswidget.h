#ifndef NUMBERBASESWIDGET_H
#define NUMBERBASESWIDGET_H

#include <QWidget>

namespace Ui {
class NumberBasesWidget;
}

class NumberBasesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NumberBasesWidget(QWidget *parent = nullptr);
    ~NumberBasesWidget();

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
