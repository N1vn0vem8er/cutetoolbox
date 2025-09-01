#include "urlcoderdecoderwidget.h"
#include "src/widgets/ui_urlcoderdecoderwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>

UrlCoderDecoderWidget::UrlCoderDecoderWidget(QWidget *parent)
    : CustomWidget(parent)
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

bool UrlCoderDecoderWidget::canOpenFiles() const
{
    return true;
}

bool UrlCoderDecoderWidget::canSaveFiles() const
{
    return true;
}

bool UrlCoderDecoderWidget::canBasicEdit() const
{
    return true;
}

bool UrlCoderDecoderWidget::canChangeFont() const
{
    return true;
}

void UrlCoderDecoderWidget::save()
{

}

void UrlCoderDecoderWidget::saveAs()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(option == TextEdits::decoded ? ui->decoded->toPlainText().toUtf8() : ui->encoded->toPlainText().toUtf8());
                file.close();
            }
        }
    }
}

void UrlCoderDecoderWidget::open()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                if(option == TextEdits::decoded)
                    ui->decoded->setPlainText(file.readAll());
                else if(option == TextEdits::encoded)
                    ui->encoded->setPlainText(file.readAll());
                file.close();
            }
        }
    }
}

void UrlCoderDecoderWidget::increaseFontSize()
{
    if(ui->decoded->hasFocus())
        ui->decoded->increaseFontSize();
    else if(ui->encoded->hasFocus())
        ui->encoded->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::decoded)
                ui->decoded->increaseFontSize();
            else if(option == TextEdits::encoded)
                ui->encoded->increaseFontSize();
        }
    }
}

void UrlCoderDecoderWidget::decreaseFontSize()
{
    if(ui->decoded->hasFocus())
        ui->decoded->decreaseFontSize();
    else if(ui->encoded->hasFocus())
        ui->encoded->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::decoded)
                ui->decoded->decreaseFontSize();
            else if(option == TextEdits::encoded)
                ui->encoded->decreaseFontSize();
        }
    }
}

void UrlCoderDecoderWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200);
        if(option == TextEdits::decoded)
            ui->decoded->setFontSize(size);
        else if(option == TextEdits::encoded)
            ui->encoded->setFontSize(size);
    }
}

void UrlCoderDecoderWidget::resetFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        if(option == TextEdits::decoded)
            ui->decoded->setFontSize(10);
        else if(option == TextEdits::encoded)
            ui->encoded->setFontSize(10);
    }
}

void UrlCoderDecoderWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::decoded)
                ui->decoded->setFont(font);
            else if(option == TextEdits::encoded)
                ui->encoded->setFont(font);
        }
    }
}

UrlCoderDecoderWidget::TextEdits UrlCoderDecoderWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton textButton(tr("Decoded"), &dialog);
    QPushButton base64Button(tr("Encoded"), &dialog);
    connect(&textButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::decoded;});
    connect(&base64Button, &QPushButton::clicked, &dialog, [&]{option = TextEdits::encoded;});
    connect(&textButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&base64Button, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&textButton);
    layout.addWidget(&base64Button);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
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
