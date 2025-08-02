#include "gzipwidget.h"
#include "src/widgets/ui_gzipwidget.h"

GZipWidget::GZipWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GZipWidget)
{
    ui->setupUi(this);
}

GZipWidget::~GZipWidget()
{
    delete ui;
}
