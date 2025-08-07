#include "qrcodegenerator.h"
#include "src/widgets/ui_qrcodegenerator.h"
#include <qrcodegen/QrCode.hpp>
#include <QFileDialog>
#include <QGraphicsRectItem>

QrCodeGenerator::QrCodeGenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QrCodeGenerator)
{
    ui->setupUi(this);
    textUrlLayout = new QFormLayout(ui->formWidget);
    textUrl = new QLineEdit(ui->formWidget);
    textUrlLayout->addWidget(textUrl);
    scene = new QGraphicsScene(ui->output);
    scene->setBackgroundBrush(QColor::fromRgb(255, 255, 255));
    ui->output->setScene(scene);
    ui->output->setRenderHint(QPainter::Antialiasing, true);
    ui->output->setMinimumSize(200, 200);
    connect(ui->generateButton, &QPushButton::clicked, this, &QrCodeGenerator::generate);
    optionSelected();
    connect(ui->saveButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getSaveFileName(this, tr("Save Qr code"), QDir::homePath());
        if(!path.isEmpty())
        {
            QImage image(400, 400, QImage::Format_RGB32);
            QPainter painter(&image);
            scene->render(&painter, QRectF(0, 0, 400, 400), scene->sceneRect());
            image.save(path);
        }
    });
}

QrCodeGenerator::~QrCodeGenerator()
{
    delete ui;
}

void QrCodeGenerator::generate()
{
    scene->clear();
    switch(ui->comboBox->currentIndex())
    {
    case 0:
    {
        const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(textUrl->text().toLatin1(), qrcodegen::QrCode::Ecc::MEDIUM);
        int size = qr.getSize();
        qreal rectSize = 1.0;
        for(int y = 0; y < size; y++)
        {
            for(int x = 0; x < size; x++)
            {
                if(qr.getModule(x,y))
                {
                    QGraphicsRectItem *rect = scene->addRect(x * rectSize, y * rectSize, rectSize, rectSize);
                    rect->setBrush(Qt::black);
                    rect->setPen(Qt::NoPen);
                }
            }
        }
        ui->output->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
        break;
    case 1:
        break;
    }
}

void QrCodeGenerator::optionSelected()
{
    switch(ui->comboBox->currentIndex())
    {
    case 0:
        ui->formWidget->setLayout(textUrlLayout);
        break;
    case 1:
        break;
    }
}

void QrCodeGenerator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->output->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

