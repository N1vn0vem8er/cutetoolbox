#include "htmlformatterwidget.h"
#include "src/widgets/ui_htmlformatterwidget.h"
#include <QFileDialog>
#include <QDomDocument>

HTMLFormatterWidget::HTMLFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HTMLFormatterWidget)
{
    ui->setupUi(this);
    ui->spinBox->setValue(4);
    connect(ui->formatButton, &QPushButton::clicked, this, &HTMLFormatterWidget::format);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copy);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &HTMLFormatterWidget::open);
}

HTMLFormatterWidget::~HTMLFormatterWidget()
{
    delete ui;
}

void HTMLFormatterWidget::format()
{
    QDomDocument doc;
    doc.setContent(ui->codeEditor->toPlainText());
    const QString formatted = doc.toString(ui->spinBox->value());
    if(!formatted.isEmpty())
    {
        ui->codeEditor->setPlainText(formatted);
    }
}

void HTMLFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
        }
    }
}
