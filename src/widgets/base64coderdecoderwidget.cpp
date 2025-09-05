#include "base64coderdecoderwidget.h"
#include "src/widgets/ui_base64coderdecoderwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>

Base64CoderDecoderWidget::Base64CoderDecoderWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::Base64CoderDecoderWidget)
{
    ui->setupUi(this);
    connect(ui->text, &QPlainTextEdit::textChanged, this, &Base64CoderDecoderWidget::encode);
    connect(ui->base64, &QPlainTextEdit::textChanged, this, &Base64CoderDecoderWidget::decode);
    connect(ui->openInputButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->text->setPlainText(file.readAll());
                openedTextFile = path;
                file.close();
            }
        }
    });
    connect(ui->openCodedButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->base64->setPlainText(file.readAll());
                openedBase64File = path;
                file.close();
            }
        }
    });
    connect(ui->copyCodedButton, &QPushButton::clicked, this, [&]{ui->base64->selectAll(); ui->base64->copy();});
    connect(ui->copyInputButton, &QPushButton::clicked, this, [&]{ui->text->selectAll(); ui->text->copy();});
    connect(ui->pasteCodedButton, &QPushButton::clicked, ui->base64, &QPlainTextEdit::paste);
    connect(ui->pasteInputButton, &QPushButton::clicked, ui->text, &QPlainTextEdit::paste);
    connect(ui->clearCodedButton, &QPushButton::clicked, ui->base64, &QPlainTextEdit::clear);
    connect(ui->clearInputButton, &QPushButton::clicked, ui->text, &QPlainTextEdit::clear);
    connect(ui->urlSafeCheckBox, &QCheckBox::clicked, this, &Base64CoderDecoderWidget::encode);
    ui->text->setAutoClosingEnabled(false);
    ui->text->setReplaceTabWithSpacesEnabled(false);
    ui->base64->setAutoClosingEnabled(false);
    ui->base64->setReplaceTabWithSpacesEnabled(false);
}

Base64CoderDecoderWidget::~Base64CoderDecoderWidget()
{
    delete ui;
}

bool Base64CoderDecoderWidget::canOpenFiles() const
{
    return true;
}

bool Base64CoderDecoderWidget::canSaveFiles() const
{
    return true;
}

bool Base64CoderDecoderWidget::canBasicEdit() const
{
    return true;
}

bool Base64CoderDecoderWidget::canChangeFont() const
{
    return true;
}

void Base64CoderDecoderWidget::save()
{
    if(!openedBase64File.isEmpty() || !openedTextFile.isEmpty())
    {
        TextEdits option = TextEdits::none;
        if(ui->text->hasFocus() && !openedTextFile.isEmpty())
            option = TextEdits::text;
        else if(ui->base64->hasFocus() && !openedBase64File.isEmpty())
            option = TextEdits::base64;
        else if(!openedTextFile.isEmpty() && !openedBase64File.isEmpty())
            option = getSelectedOption();
        else if(!openedTextFile.isEmpty())
            option = TextEdits::text;
        else if(openedBase64File.isEmpty())
            option = TextEdits::base64;
        if(option != TextEdits::none)
        {
            if(option == TextEdits::text)
            {
                QFile file(openedTextFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->text->toPlainText().toUtf8());
                    file.close();
                }
            }
            else if(option == TextEdits::base64)
            {
                QFile file(openedBase64File);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->base64->toPlainText().toUtf8());
                    file.close();
                }
            }
        }
    }
    else
        saveAs();
}

void Base64CoderDecoderWidget::saveAs()
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
                if(option == TextEdits::text)
                {
                    file.write(ui->text->toPlainText().toUtf8());
                    openedTextFile = path;
                }
                else if(option == TextEdits::base64)
                {
                    file.write(ui->base64->toPlainText().toUtf8());
                    openedBase64File = path;
                }
                file.close();
            }
        }
    }
}

void Base64CoderDecoderWidget::open()
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
                if(option == TextEdits::text)
                {
                    ui->text->setPlainText(file.readAll());
                    openedTextFile = path;
                }
                else if(option == TextEdits::base64)
                {
                    ui->base64->setPlainText(file.readAll());
                    openedBase64File = path;
                }
                file.close();
            }
        }
    }
}

void Base64CoderDecoderWidget::increaseFontSize()
{
    if(ui->text->hasFocus())
        ui->text->increaseFontSize();
    else if(ui->base64->hasFocus())
        ui->base64->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::text)
                ui->text->increaseFontSize();
            else if(option == TextEdits::base64)
                ui->base64->increaseFontSize();
        }
    }
}

void Base64CoderDecoderWidget::decreaseFontSize()
{
    if(ui->text->hasFocus())
        ui->text->decreaseFontSize();
    else if(ui->base64->hasFocus())
        ui->base64->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::text)
                ui->text->decreaseFontSize();
            else if(option == TextEdits::base64)
                ui->base64->decreaseFontSize();
        }
    }
}

void Base64CoderDecoderWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200);
        if(option == TextEdits::text)
            ui->text->setFontSize(size);
        else if(option == TextEdits::base64)
            ui->base64->setFontSize(size);
    }
}

void Base64CoderDecoderWidget::resetFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        if(option == TextEdits::text)
            ui->text->setFontSize(10);
        else if(option == TextEdits::base64)
            ui->base64->setFontSize(10);
    }
}

void Base64CoderDecoderWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::text)
                ui->text->setFont(font);
            else if(option == TextEdits::base64)
                ui->base64->setFont(font);
        }
    }
}

Base64CoderDecoderWidget::TextEdits Base64CoderDecoderWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton textButton(tr("Text"), &dialog);
    QPushButton base64Button(tr("Base64"), &dialog);
    connect(&textButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::text;});
    connect(&base64Button, &QPushButton::clicked, &dialog, [&]{option = TextEdits::base64;});
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

void Base64CoderDecoderWidget::encode()
{
    if(!coding)
    {
        coding = true;
        QByteArray base64 = ui->text->toPlainText().toUtf8().toBase64();
        if(ui->urlSafeCheckBox->isChecked())
        {
            base64.replace("+", "-");
            base64.replace("/", "_");
            base64.removeIf([](auto byte){return byte == '=';});
        }
        ui->base64->setPlainText(base64);
    }
    else
    {
        coding = false;
    }
}

void Base64CoderDecoderWidget::decode()
{
    if(!coding)
    {
        coding = true;
        QByteArray base64 = QByteArray::fromBase64(ui->base64->toPlainText().toUtf8());
        if(ui->urlSafeCheckBox->isChecked())
        {
            base64.replace("-", "+");
            base64.replace("_", "/");
            while(base64.length() % 4 != 0)
            {
                base64.append('=');
            }
        }
        ui->text->setPlainText(base64);
    }
    else
    {
        coding = false;
    }
}
