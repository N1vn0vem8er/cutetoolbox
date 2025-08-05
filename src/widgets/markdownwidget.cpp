#include "markdownwidget.h"
#include "src/widgets/ui_markdownwidget.h"

MarkdownWidget::MarkdownWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MarkdownWidget)
{
    ui->setupUi(this);
}

MarkdownWidget::~MarkdownWidget()
{
    delete ui;
}

void MarkdownWidget::displayDocument()
{

}
