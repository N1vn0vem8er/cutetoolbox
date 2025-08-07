#ifndef CHMODCALCULATORWIDGET_H
#define CHMODCALCULATORWIDGET_H

#include <QWidget>

namespace Ui {
class ChmodCalculatorWidget;
}

class ChmodCalculatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChmodCalculatorWidget(QWidget *parent = nullptr);
    ~ChmodCalculatorWidget();

private:
    Ui::ChmodCalculatorWidget *ui;
};

#endif // CHMODCALCULATORWIDGET_H
