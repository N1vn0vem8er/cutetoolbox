#include "hashwidget.h"
#include "src/widgets/ui_hashwidget.h"
#include <QCryptographicHash>
#include <QFileDialog>
#include <QPalette>

HashWidget::HashWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HashWidget)
{
    ui->setupUi(this);
    for(const auto& i : hashTypes)
    {
        ui->comboBox->addItem(i);
    }
    connect(ui->clearInpututton, &QPushButton::clicked, ui->input, &QPlainTextEdit::clear);
    connect(ui->copyInputButton, &QPushButton::clicked, ui->input, [&]{ui->input->selectAll(); ui->input->copy();});
    connect(ui->pasteInputButton, &QPushButton::clicked, ui->input, &QPlainTextEdit::paste);
    connect(ui->openInputButton, &QPushButton::clicked, this, &HashWidget::open);
    connect(ui->input, &QPlainTextEdit::textChanged, this, &HashWidget::calculateHash);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &HashWidget::calculateHash);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &HashWidget::calculateHash);
    connect(ui->clearIntegrityCheckButton, &QPushButton::clicked, ui->lineEdit, &QLineEdit::clear);
    connect(ui->pasteIntegrityCheckButton, &QPushButton::clicked, ui->lineEdit, &QLineEdit::paste);
    connect(ui->copyOutputButton, &QPushButton::clicked, this, [&]{ui->output->selectAll(); ui->output->copy();});
}

HashWidget::~HashWidget()
{
    delete ui;
}

void HashWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->input->setPlainText(file.readAll());
            file.close();
        }
    }
}

void HashWidget::calculateHash()
{
    const QByteArray input = ui->input->toPlainText().toLatin1();
    switch(ui->comboBox->currentIndex())
    {
    case HashTypes::MD5:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Md5).toHex());
        break;
    case HashTypes::SHA1:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Sha1).toHex());
        break;
    case HashTypes::SHA256:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Sha256).toHex());
        break;
    case HashTypes::SHA512:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Sha512).toHex());
        break;
    case HashTypes::SHA3_256:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Sha3_256).toHex());
        break;
    case HashTypes::SHA3_512:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Sha3_512).toHex());
        break;
    case HashTypes::KECCAK256:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Keccak_256).toHex());
        break;
    case HashTypes::KECCAK512:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Keccak_512).toHex());
        break;
    case HashTypes::BLAKE2b256:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Blake2b_256).toHex());
        break;
    case HashTypes::BLAKE2b512:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Blake2b_512).toHex());
        break;
    case HashTypes::BLAKE2s256:
        ui->output->setPlainText(QCryptographicHash::hash(input, QCryptographicHash::Blake2s_256).toHex());
        break;
    }
    if(!ui->lineEdit->text().isEmpty())
    {
        const QString hash = ui->lineEdit->text();
        QPalette palette = ui->lineEdit->palette();
        palette.setColor(QPalette::Base, hash == ui->output->toPlainText() ? Qt::green : Qt::red);
        palette.setColor(QPalette::Text, hash == ui->output->toPlainText() ? Qt::black : Qt::white);
        ui->lineEdit->setPalette(palette);
    }
    else
    {
         ui->lineEdit->setPalette(QApplication::palette(ui->lineEdit));
    }
}
