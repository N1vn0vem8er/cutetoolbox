#include "passwordverifierwidget.h"
#include "src/widgets/ui_passwordverifierwidget.h"

PasswordVerifierWidget::PasswordVerifierWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::PasswordVerifierWidget)
{
    ui->setupUi(this);
}

PasswordVerifierWidget::~PasswordVerifierWidget()
{
    delete ui;
}
