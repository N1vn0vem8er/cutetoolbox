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
};

#endif // NUMBERBASESWIDGET_H
