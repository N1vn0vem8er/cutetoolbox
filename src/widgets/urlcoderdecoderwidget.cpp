#include "urlcoderdecoderwidget.h"
#include "src/widgets/ui_urlcoderdecoderwidget.h"
#include <QFileDialog>

UrlCoderDecoderWidget::UrlCoderDecoderWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UrlCoderDecoderWidget)
{
    ui->setupUi(this);
    ui->spacePlusButton->setChecked(true);
    connect(ui->decoded, &QPlainTextEdit::textChanged, this, &UrlCoderDecoderWidget::encode);
    connect(ui->encoded, &QPlainTextEdit::textChanged, this, &UrlCoderDecoderWidget::decode);
    connect(ui->openDecodedButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->decoded->setPlainText(file.readAll());
                file.close();
            }
        }
    });
    connect(ui->openEncodedButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->encoded->setPlainText(file.readAll());
                file.close();
            }
        }
    });
    connect(ui->copyEncodedButton, &QPushButton::clicked, this, [&]{ui->encoded->selectAll(); ui->encoded->copy();});
    connect(ui->copyDecodedButton, &QPushButton::clicked, this, [&]{ui->decoded->selectAll(); ui->decoded->copy();});
    connect(ui->pasteEncodedButton, &QPushButton::clicked, ui->encoded, &QPlainTextEdit::paste);
    connect(ui->pasteDecodedButton, &QPushButton::clicked, ui->decoded, &QPlainTextEdit::paste);
    connect(ui->clearEncodedButton, &QPushButton::clicked, ui->encoded, &QPlainTextEdit::clear);
    connect(ui->clearDecodedButton, &QPushButton::clicked, ui->decoded, &QPlainTextEdit::clear);
    connect(ui->space20Button, &QRadioButton::clicked, this, &UrlCoderDecoderWidget::encode);
    connect(ui->spacePlusButton, &QRadioButton::clicked, this, &UrlCoderDecoderWidget::encode);
    connect(ui->space20Button, &QRadioButton::clicked, this, &UrlCoderDecoderWidget::decode);
    connect(ui->spacePlusButton, &QRadioButton::clicked, this, &UrlCoderDecoderWidget::decode);
}

UrlCoderDecoderWidget::~UrlCoderDecoderWidget()
{
    delete ui;
}

void UrlCoderDecoderWidget::encode()
{
    if(!coding)
    {
        coding = true;
        ui->encoded->setPlainText(ui->decoded->toPlainText().replace(" ", ui->spacePlusButton->isChecked() ? "+" : "%20"));
    }
    else
    {
        coding = false;
    }
}

void UrlCoderDecoderWidget::decode()
{
    if(!coding)
    {
        coding = true;
        ui->decoded->setPlainText(ui->encoded->toPlainText().replace(ui->spacePlusButton->isChecked() ? "+" : "%20", " "));
    }
    else
    {
        coding = false;
    }
}
