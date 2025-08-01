#include "randomtextgeneratorwidget.h"
#include "src/widgets/ui_randomtextgeneratorwidget.h"

RandomTextGeneratorWidget::RandomTextGeneratorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RandomTextGeneratorWidget)
{
    ui->setupUi(this);
}

RandomTextGeneratorWidget::~RandomTextGeneratorWidget()
{
    delete ui;
}
