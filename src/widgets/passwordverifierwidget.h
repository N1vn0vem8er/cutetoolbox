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
    bool canBasicEdit() const override;

private:
    Ui::PasswordVerifierWidget *ui;

private slots:
    void checkPassword();
};

#endif // PASSWORDVERIFIERWIDGET_H
