#ifndef CONTRASTCHECKERWIDGET_H
#define CONTRASTCHECKERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ContrastCheckerWidget;
}

class ContrastCheckerWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ContrastCheckerWidget(QWidget *parent = nullptr);
    ~ContrastCheckerWidget();

private:
    Ui::ContrastCheckerWidget *ui;
    QColor textColor {Qt::black};
    QColor backgroundColor {Qt::white};

private:
    void updateExample();
    void calculateContrastRatio();

private slots:
    void selectTextColor();
    void selectBackgroundColor();
};

#endif // CONTRASTCHECKERWIDGET_H
