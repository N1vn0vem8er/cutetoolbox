#include "imageformatconverterwidget.h"
#include "src/widgets/ui_imageformatconverterwidget.h"
#include <QFileDialog>
#include <QImageWriter>

ImageFormatConverterWidget::ImageFormatConverterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ImageFormatConverterWidget)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &ImageFormatConverterWidget::open);
    connect(ui->convertButton, &QPushButton::clicked, this, &ImageFormatConverterWidget::convert);
    for(const QString& i : QImageWriter::supportedImageFormats())
    {
        ui->comboBox->addItem(i);
    }
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

void ImageFormatConverterWidget::convert()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), QString("*.%1").arg(ui->comboBox->currentText()));
    if(!path.isEmpty())
    {
        image.save(path, ui->comboBox->currentText().toUtf8());
    }
}
