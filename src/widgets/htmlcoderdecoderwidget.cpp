#include "htmlcoderdecoderwidget.h"
#include "src/widgets/ui_htmlcoderdecoderwidget.h"

#include <QFileDialog>

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

}

void HTMLCoderDecoderWidget::saveAs()
{
    int option;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton htmlButton(tr("Html"), &dialog);
    QPushButton encodedButton(tr("Encoded"), &dialog);
    connect(&htmlButton, &QPushButton::clicked, &dialog, [&]{option = 0;});
    connect(&encodedButton, &QPushButton::clicked, &dialog, [&]{option = 1;});
    connect(&htmlButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&encodedButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&htmlButton);
    layout.addWidget(&encodedButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(option == 0 ? ui->html->toPlainText().toUtf8() : ui->encoded->toPlainText().toUtf8());
                file.close();
            }
        }
    }
}

void HTMLCoderDecoderWidget::open()
{
    int option;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton htmlButton(tr("Html"), &dialog);
    QPushButton encodedButton(tr("Encoded"), &dialog);
    connect(&htmlButton, &QPushButton::clicked, &dialog, [&]{option = 0;});
    connect(&encodedButton, &QPushButton::clicked, &dialog, [&]{option = 1;});
    connect(&htmlButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&encodedButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&htmlButton);
    layout.addWidget(&encodedButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                if(option == 0)
                    ui->html->setPlainText(file.readAll());
                else
                    ui->encoded->setPlainText(file.readAll());
                file.close();
            }
        }
    }
}

void HTMLCoderDecoderWidget::copy()
{

}

void HTMLCoderDecoderWidget::cut()
{

}

void HTMLCoderDecoderWidget::paste()
{

}

void HTMLCoderDecoderWidget::selectAll()
{

}

void HTMLCoderDecoderWidget::deleteText()
{

}

void HTMLCoderDecoderWidget::deleteAllText()
{

}

void HTMLCoderDecoderWidget::increaseFontSize()
{

}

void HTMLCoderDecoderWidget::decreaseFontSize()
{

}

void HTMLCoderDecoderWidget::setFontSize()
{

}

void HTMLCoderDecoderWidget::resetFontSize()
{

}

void HTMLCoderDecoderWidget::setFont()
{

}

