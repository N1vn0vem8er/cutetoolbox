#include "htmlcoderdecoderwidget.h"
#include "src/widgets/ui_htmlcoderdecoderwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>

HTMLCoderDecoderWidget::HTMLCoderDecoderWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::HTMLCoderDecoderWidget)
{
    ui->setupUi(this);
    setName(tr("Html Encoder/Decoder"));
    connect(ui->html, &QPlainTextEdit::textChanged, this, &HTMLCoderDecoderWidget::encode);
    connect(ui->encoded, &QPlainTextEdit::textChanged, this, &HTMLCoderDecoderWidget::decode);
    connect(ui->openHtmlButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->html->setPlainText(file.readAll());
                openedHtmlFile = path;
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
                ui->encoded->setPlainText(file.readAll());
                openedEncodedFile = path;
                file.close();
            }
        }
    });
    connect(ui->copyCodedButton, &QPushButton::clicked, this, [&]{ui->encoded->selectAll(); ui->encoded->copy();});
    connect(ui->copyHtmlButton, &QPushButton::clicked, this, [&]{ui->html->selectAll(); ui->html->copy();});
    connect(ui->pasteCodedButton, &QPushButton::clicked, ui->encoded, &QPlainTextEdit::paste);
    connect(ui->pasteHtmlButton, &QPushButton::clicked, ui->html, &QPlainTextEdit::paste);
    connect(ui->clearCodedButton, &QPushButton::clicked, ui->encoded, &QPlainTextEdit::clear);
    connect(ui->clearHtmlButton, &QPushButton::clicked, ui->html, &QPlainTextEdit::clear);
}

HTMLCoderDecoderWidget::~HTMLCoderDecoderWidget()
{
    delete ui;
}

void HTMLCoderDecoderWidget::encode()
{
    if(!coding)
    {
        coding = true;
        ui->encoded->setPlainText(ui->html->toPlainText().replace("&", "&amp;").replace(">", "&gt;").replace("<", "&lt;").replace("\"", "&quot;").replace("'", "&#x27;"));
    }
    else
    {
        coding = false;
    }
}

void HTMLCoderDecoderWidget::decode()
{
    if(!coding)
    {
        coding = true;
        ui->html->setPlainText(ui->encoded->toPlainText().replace("&gt;", ">").replace("&lt;", "<").replace("&quot;", "\"").replace("&#x27;", "'").replace("&amp;", "&"));
    }
    else
    {
        coding = false;
    }
}

bool HTMLCoderDecoderWidget::canOpenFiles() const
{
    return true;
}

bool HTMLCoderDecoderWidget::canSaveFiles() const
{
    return true;
}

bool HTMLCoderDecoderWidget::canBasicEdit() const
{
    return true;
}

bool HTMLCoderDecoderWidget::canChangeFont() const
{
    return true;
}

void HTMLCoderDecoderWidget::save()
{
    if(!openedEncodedFile.isEmpty() || !openedHtmlFile.isEmpty())
    {
        TextEdits option = TextEdits::none;
        if(ui->html->hasFocus() && !openedHtmlFile.isEmpty())
            option = TextEdits::html;
        else if(ui->encoded->hasFocus() && !openedEncodedFile.isEmpty())
            option = TextEdits::encoded;
        else if(!openedHtmlFile.isEmpty() && !openedEncodedFile.isEmpty())
            option = getSelectedOption();
        else if(!openedHtmlFile.isEmpty())
            option = TextEdits::html;
        else if(openedEncodedFile.isEmpty())
            option = TextEdits::encoded;
        if(option != TextEdits::none)
        {
            if(option == TextEdits::html)
            {
                QFile file(openedHtmlFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->html->toPlainText().toUtf8());
                    file.close();
                }
            }
            else if(option == TextEdits::encoded)
            {
                QFile file(openedEncodedFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->encoded->toPlainText().toUtf8());
                    file.close();
                }
            }
        }
    }
    else
        saveAs();
}

void HTMLCoderDecoderWidget::saveAs()
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
                if(option == TextEdits::html)
                {
                    file.write(ui->html->toPlainText().toUtf8());
                    openedHtmlFile = path;
                }
                else if(option == TextEdits::encoded)
                {
                    file.write(ui->encoded->toPlainText().toUtf8());
                    openedEncodedFile = path;
                }
                file.close();
            }
        }
    }
}

void HTMLCoderDecoderWidget::open()
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
                {
                    ui->html->setPlainText(file.readAll());
                    openedHtmlFile = path;
                }
                else if(option == TextEdits::encoded)
                {
                    ui->encoded->setPlainText(file.readAll());
                    openedEncodedFile = path;
                }
                file.close();
            }
        }
    }
}

void HTMLCoderDecoderWidget::increaseFontSize()
{
    if(ui->html->hasFocus())
        ui->html->increaseFontSize();
    else if(ui->encoded->hasFocus())
        ui->encoded->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::html)
                ui->html->increaseFontSize();
            else if(option == TextEdits::encoded)
                ui->encoded->increaseFontSize();
        }
    }
}

void HTMLCoderDecoderWidget::decreaseFontSize()
{
    if(ui->html->hasFocus())
        ui->html->decreaseFontSize();
    else if(ui->encoded->hasFocus())
        ui->encoded->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::html)
                ui->html->decreaseFontSize();
            else if(option == TextEdits::encoded)
                ui->encoded->decreaseFontSize();
        }
    }
}

void HTMLCoderDecoderWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200);
        if(option == TextEdits::html)
            ui->html->setFontSize(size);
        else if(option == TextEdits::encoded)
            ui->encoded->setFontSize(size);
    }
}

void HTMLCoderDecoderWidget::resetFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        if(option == TextEdits::html)
            ui->html->setFontSize(10);
        else if(option == TextEdits::encoded)
            ui->encoded->setFontSize(10);
    }
}

void HTMLCoderDecoderWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::html)
                ui->html->setFont(font);
            else if(option == TextEdits::encoded)
                ui->encoded->setFont(font);
        }
    }
}

HTMLCoderDecoderWidget::TextEdits HTMLCoderDecoderWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton htmlButton(tr("Html"), &dialog);
    QPushButton encodedButton(tr("Encoded"), &dialog);
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

