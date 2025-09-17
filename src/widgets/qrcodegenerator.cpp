#include "qrcodegenerator.h"
#include "src/widgets/ui_qrcodegenerator.h"
#include <qrcodegen/QrCode.hpp>
#include <QFileDialog>
#include <QGraphicsRectItem>

QrCodeGenerator::QrCodeGenerator(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::QrCodeGenerator)
{
    ui->setupUi(this);
    setName(tr("Qr Code Generator"));
    textUrlWidget = new QWidget(ui->stackedWidget);
    QFormLayout* textUrlLayout = nullptr;
    textUrlLayout = new QFormLayout(textUrlWidget);
    textUrl = new QLineEdit(textUrlWidget);
    textUrl->setClearButtonEnabled(true);
    textUrlLayout->addWidget(textUrl);
    textUrlWidget->setLayout(textUrlLayout);
    ui->stackedWidget->addWidget(textUrlWidget);

    wifiNetworkWidget = new QWidget(ui->stackedWidget);
    QFormLayout* wifiNetworkLayout = new QFormLayout(wifiNetworkWidget);
    networkName = new QLineEdit(wifiNetworkWidget);
    networkName->setClearButtonEnabled(true);
    encryptionType = new QComboBox(wifiNetworkWidget);
    encryptionType->addItems({tr("WPA"), tr("WEP"), tr("No encryption")});
    networkPassword = new QLineEdit(wifiNetworkWidget);
    networkPassword->setClearButtonEnabled(true);
    wifiNetworkLayout->addRow(tr("Network Name"), networkName);
    wifiNetworkLayout->addRow(tr("Network Encryption"), encryptionType);
    wifiNetworkLayout->addRow(tr("Network Password"), networkPassword);
    wifiNetworkWidget->setLayout(wifiNetworkLayout);
    ui->stackedWidget->addWidget(wifiNetworkWidget);

    contactWidget = new QWidget(ui->stackedWidget);
    QFormLayout* contactLayout = new QFormLayout(contactWidget);
    firstName = new QLineEdit(contactWidget);
    firstName->setClearButtonEnabled(true);
    lastName = new QLineEdit(contactWidget);
    lastName->setClearButtonEnabled(true);
    phoneNumber = new QLineEdit(contactWidget);
    phoneNumber->setClearButtonEnabled(true);
    email = new QLineEdit(contactWidget);
    email->setClearButtonEnabled(true);
    contactLayout->addRow(tr("First Name"), firstName);
    contactLayout->addRow(tr("Last Name"), lastName);
    contactLayout->addRow(tr("Phone Number"), phoneNumber);
    contactLayout->addRow(tr("Email"), email);
    contactWidget->setLayout(contactLayout);
    ui->stackedWidget->addWidget(contactWidget);

    scene = new QGraphicsScene(ui->output);
    scene->setBackgroundBrush(QColor::fromRgb(255, 255, 255));
    ui->output->setScene(scene);
    ui->output->setRenderHint(QPainter::Antialiasing, true);
    ui->output->setMinimumSize(200, 200);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &QrCodeGenerator::optionSelected);
    connect(ui->generateButton, &QPushButton::clicked, this, &QrCodeGenerator::generate);
    optionSelected(0);
    connect(ui->saveButton, &QPushButton::clicked, this, &QrCodeGenerator::saveAs);
}

QrCodeGenerator::~QrCodeGenerator()
{
    delete ui;
}

bool QrCodeGenerator::canSaveFiles() const
{
    return true;
}

bool QrCodeGenerator::canBasicEdit() const
{
    return true;
}

void QrCodeGenerator::save()
{
    if(!openedFile.isEmpty())
    {
        QImage image(400, 400, QImage::Format_RGB32);
        QPainter painter(&image);
        scene->render(&painter, QRectF(0, 0, 400, 400), scene->sceneRect());
        image.save(openedFile);
    }
    else
        saveAs();
}

void QrCodeGenerator::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save Qr code"), QDir::homePath());
    if(!path.isEmpty())
    {
        QImage image(400, 400, QImage::Format_RGB32);
        QPainter painter(&image);
        scene->render(&painter, QRectF(0, 0, 400, 400), scene->sceneRect());
        image.save(path);
        openedFile = path;
    }
}

void QrCodeGenerator::generate()
{
    scene->clear();
    try
    {
        ui->errorLabel->clear();
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
        {
            ui->errorLabel->clear();
            const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(QString("WIFI:S:%1;T:%2;P:%3;;")
                                                                           .arg(networkName->text(),
                                                                                encryptionType->currentIndex() < encryptionType->count() - 1 ? encryptionType->currentText() : "nopass",
                                                                                networkPassword->text()).toLatin1(),
                                                                       qrcodegen::QrCode::Ecc::MEDIUM);
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
        case 2:
        {
            const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(QString("BEGIN:VCARD\nVERSION:3.0\nFN:%1 %2\nTEL:%3\nEMAIL:%4\nEND:VCARD")
                                                                           .arg(firstName->text(), lastName->text(), phoneNumber->text(), email->text()).toLatin1(),
                                                                       qrcodegen::QrCode::Ecc::MEDIUM);
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
        }
    }
    catch(const std::exception& e)
    {
        ui->errorLabel->setText(e.what());
    }
}

void QrCodeGenerator::optionSelected(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void QrCodeGenerator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->output->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

