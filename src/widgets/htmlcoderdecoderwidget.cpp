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

