#include "textdiffwidget.h"
#include "src/widgets/ui_textdiffwidget.h"

TextDiffWidget::TextDiffWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextDiffWidget)
{
    ui->setupUi(this);
}

TextDiffWidget::~TextDiffWidget()
{
    delete ui;
}
