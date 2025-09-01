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
    ui->html->installEventFilter(this);
    ui->encoded->installEventFilter(this);
}

HTMLCoderDecoderWidget::~HTMLCoderDecoderWidget()
{
    delete ui;
}

bool HTMLCoderDecoderWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::FocusOut)
    {
        if(watched == ui->html)
            lastInFocus = TextEdits::html;
        else if(watched == ui->encoded)
            lastInFocus = TextEdits::encoded;
    }
    return QObject::eventFilter(watched, event);
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
                file.write(option == TextEdits::html ? ui->html->toPlainText().toUtf8() : ui->encoded->toPlainText().toUtf8());
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
                    ui->html->setPlainText(file.readAll());
                else if(option == TextEdits::encoded)
                    ui->encoded->setPlainText(file.readAll());
                file.close();
            }
        }
    }
}

void HTMLCoderDecoderWidget::deleteText()
{
    if(ui->html->hasFocus())
        ui->html->deleteSelected();
    else if(ui->encoded->hasFocus())
        ui->encoded->deleteSelected();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::html)
                ui->html->deleteSelected();
            else if(option == TextEdits::encoded)
                ui->encoded->deleteSelected();
        }
    }
}

void HTMLCoderDecoderWidget::deleteAllText()
{
    if(ui->html->hasFocus())
        ui->html->deleteAll();
    else if(ui->encoded->hasFocus())
        ui->encoded->deleteAll();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::html)
                ui->html->deleteAll();
            else if(option == TextEdits::encoded)
                ui->encoded->deleteAll();
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

