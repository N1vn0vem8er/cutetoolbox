#include "hashwidget.h"
#include "config.h"
#include "src/widgets/ui_hashwidget.h"
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QPalette>
#include <qsettings.h>

HashWidget::HashWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::HashWidget)
{
    ui->setupUi(this);
    setName(tr("Hash Generator"));
    for(const auto& i : hashTypes)
    {
        ui->comboBox->addItem(i);
    }
    QSettings settings(Config::settingsName);
    ui->comboBox->setCurrentIndex(settings.value("hashGenerator.hashType", 0).toInt());
    connect(ui->clearInpututton, &QPushButton::clicked, ui->input, &QPlainTextEdit::clear);
    connect(ui->copyInputButton, &QPushButton::clicked, ui->input, [&]{ui->input->selectAll(); ui->input->copy();});
    connect(ui->pasteInputButton, &QPushButton::clicked, ui->input, &QPlainTextEdit::paste);
    connect(ui->openInputButton, &QPushButton::clicked, this, &HashWidget::open);
    connect(ui->input, &QPlainTextEdit::textChanged, this, &HashWidget::calculateHash);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &HashWidget::calculateHash);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &HashWidget::calculateHash);
    connect(ui->pasteIntegrityCheckButton, &QPushButton::clicked, ui->lineEdit, &QLineEdit::paste);
    connect(ui->copyOutputButton, &QPushButton::clicked, this, [&]{ui->output->selectAll(); ui->output->copy();});
    ui->input->setReplaceTabWithSpacesEnabled(false);
    ui->input->setAutoClosingEnabled(false);
    ui->output->setAutoClosingEnabled(false);
    ui->output->setReplaceTabWithSpacesEnabled(false);
    calculateHash();
}

HashWidget::~HashWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("hashGenerator.hashType", ui->comboBox->currentIndex());
    delete ui;
}

bool HashWidget::canOpenFiles() const
{
    return true;
}

bool HashWidget::canBasicEdit() const
{
    return true;
}

bool HashWidget::canChangeFont() const
{
    return true;
}

void HashWidget::increaseFontSize()
{
    if(ui->input->hasFocus())
        ui->input->increaseFontSize();
    else if(ui->output->hasFocus())
        ui->output->increaseFontSize();
}

void HashWidget::decreaseFontSize()
{
    if(ui->input->hasFocus())
        ui->input->decreaseFontSize();
    else if(ui->output->hasFocus())
        ui->output->decreaseFontSize();
}

void HashWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
        if(ok)
        {
            if(option == TextEdits::input)
                ui->input->setFontSize(size);
            else if(option == TextEdits::output)
                ui->output->setFontSize(size);
        }
    }
}

void HashWidget::resetFontSize()
{
    if(ui->input->hasFocus())
        ui->input->setFontSize(10);
    else if(ui->output->hasFocus())
        ui->output->setFontSize(10);
}

void HashWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::input)
                ui->input->setFont(font);
            else if(option == TextEdits::output)
                ui->output->setFont(font);
        }
    }
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
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

QString HashWidget::getOpenedFileName() const
{
    return openedFile;
}

HashWidget::TextEdits HashWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton inputButton(tr("Input"), &dialog);
    QPushButton outputButton(tr("Output"), &dialog);
    connect(&inputButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::input;});
    connect(&outputButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::output;});
    connect(&inputButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&outputButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&inputButton);
    layout.addWidget(&outputButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
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
