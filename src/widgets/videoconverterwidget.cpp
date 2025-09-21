#include "videoconverterwidget.h"
#include "src/widgets/ui_videoconverterwidget.h"

VideoConverterWidget::VideoConverterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::VideoConverterWidget)
{
    ui->setupUi(this);
    setName(tr("Video Format Converter"));
}

VideoConverterWidget::~VideoConverterWidget()
{
    delete ui;
}
