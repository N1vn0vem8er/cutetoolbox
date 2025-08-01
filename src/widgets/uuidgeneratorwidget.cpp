#include "uuidgeneratorwidget.h"
#include "src/widgets/ui_uuidgeneratorwidget.h"

UUIDGeneratorWidget::UUIDGeneratorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UUIDGeneratorWidget)
{
    ui->setupUi(this);
}

UUIDGeneratorWidget::~UUIDGeneratorWidget()
{
    delete ui;
}
