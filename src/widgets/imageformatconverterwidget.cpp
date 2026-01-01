#include "imageformatconverterwidget.h"
#include "src/widgets/ui_imageformatconverterwidget.h"
#include <QFileDialog>
#include <QImageWriter>

ImageFormatConverterWidget::ImageFormatConverterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ImageFormatConverterWidget)
{
    ui->setupUi(this);
    setName(tr("Image Format Converter"));
    connect(ui->openButton, &QPushButton::clicked, this, &ImageFormatConverterWidget::open);
    connect(ui->convertButton, &QPushButton::clicked, this, &ImageFormatConverterWidget::convert);
    const auto supportedFormats = QImageWriter::supportedImageFormats();
    for(const QString& i : supportedFormats)
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
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath());
    if(!path.isEmpty())
    {
        image.load(path);
        if(!image.isNull())
        {
            ui->graphicsView->setImage(image);
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void ImageFormatConverterWidget::close()
{
    openedFile.clear();
    image.load(openedFile);
    ui->graphicsView->setImage(image);
    emit opened(openedFile);
}

QString ImageFormatConverterWidget::getOpenedFileName() const
{
    return openedFile;
}

void ImageFormatConverterWidget::convert()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), QString("*.%1").arg(ui->comboBox->currentText()));
    if(!path.isEmpty())
    {
        image.save(path, ui->comboBox->currentText().toUtf8());
        emit saved(tr("Converted: %1").arg(path));
    }
}
