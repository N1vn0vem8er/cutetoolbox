#ifndef PASSWORDVERIFIERWIDGET_H
#define PASSWORDVERIFIERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class PasswordVerifierWidget;
}

class PasswordVerifierWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit PasswordVerifierWidget(QWidget *parent = nullptr);
    ~PasswordVerifierWidget();

private:
    Ui::PasswordVerifierWidget *ui;
};

#endif // PASSWORDVERIFIERWIDGET_H
