#ifndef CHMODCALCULATORWIDGET_H
#define CHMODCALCULATORWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ChmodCalculatorWidget;
}

class ChmodCalculatorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ChmodCalculatorWidget(QWidget *parent = nullptr);
    ~ChmodCalculatorWidget();
    bool canBasicEdit() const override;
    void deleteText() override;
    void deleteAllText() override;

private:
    Ui::ChmodCalculatorWidget *ui;

private slots:
    void generate();
    void updateCheckboxesNumeric();
    void updateCheckBoxesSymbolic();
};

#endif // CHMODCALCULATORWIDGET_H
