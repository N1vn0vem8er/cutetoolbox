#include "imageformatconverterwidget.h"
#include "src/widgets/ui_imageformatconverterwidget.h"

#include <QFileDialog>

ImageFormatConverterWidget::ImageFormatConverterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ImageFormatConverterWidget)
{
    ui->setupUi(this);
}

ImageFormatConverterWidget::~ImageFormatConverterWidget()
{
    delete ui;
}

bool ImageFormatConverterWidget::canOpenFiles() const
{
    return true;
}

void ImageFormatConverterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
    if(!path.isEmpty())
    {
        image.load(path);
        if(!image.isNull())
        {
            ui->graphicsView->setImage(image);
        }
    }
}
