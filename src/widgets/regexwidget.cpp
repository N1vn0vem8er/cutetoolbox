#include "regexwidget.h"
#include "src/widgets/ui_regexwidget.h"

RegexWidget::RegexWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegexWidget)
{
    ui->setupUi(this);
}

RegexWidget::~RegexWidget()
{
    delete ui;
}
