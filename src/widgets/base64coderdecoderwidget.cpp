#include "base64coderdecoderwidget.h"
#include "src/widgets/ui_base64coderdecoderwidget.h"
#include <QFileDialog>

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
                file.write(option == TextEdits::html ? ui->text->toPlainText().toUtf8() : ui->base64->toPlainText().toUtf8());
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
                if(option == TextEdits::html)
                    ui->text->setPlainText(file.readAll());
                else if(option == TextEdits::encoded)
                    ui->base64->setPlainText(file.readAll());
                file.close();
            }
        }
    }
}

void Base64CoderDecoderWidget::copy()
{

}

void Base64CoderDecoderWidget::cut()
{

}

void Base64CoderDecoderWidget::paste()
{

}

void Base64CoderDecoderWidget::selectAll()
{

}

void Base64CoderDecoderWidget::deleteText()
{

}

void Base64CoderDecoderWidget::deleteAllText()
{

}

void Base64CoderDecoderWidget::undo()
{

}

void Base64CoderDecoderWidget::redo()
{

}

void Base64CoderDecoderWidget::increaseFontSize()
{

}

void Base64CoderDecoderWidget::decreaseFontSize()
{

}

void Base64CoderDecoderWidget::setFontSize()
{

}

void Base64CoderDecoderWidget::resetFontSize()
{

}

void Base64CoderDecoderWidget::setFont()
{

}

Base64CoderDecoderWidget::TextEdits Base64CoderDecoderWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton htmlButton(tr("Text"), &dialog);
    QPushButton encodedButton(tr("Base64"), &dialog);
    connect(&htmlButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::html;});
    connect(&encodedButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::encoded;});
    connect(&htmlButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&encodedButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&htmlButton);
    layout.addWidget(&encodedButton);
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
