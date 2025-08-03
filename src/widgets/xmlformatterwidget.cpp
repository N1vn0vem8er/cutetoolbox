#include "xmlformatterwidget.h"
#include "src/widgets/ui_xmlformatterwidget.h"
#include <QFileDialog>
#include <QDomDocument>

XMLFormatterWidget::XMLFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::XMLFormatterWidget)
{
    ui->setupUi(this);
    ui->spinBox->setValue(4);
    connect(ui->formatButton, &QPushButton::clicked, this, &XMLFormatterWidget::format);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copy);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &XMLFormatterWidget::open);
}

XMLFormatterWidget::~XMLFormatterWidget()
{
    delete ui;
}

void XMLFormatterWidget::open()
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

void XMLFormatterWidget::format()
{
    QDomDocument doc;
    doc.setContent(ui->codeEditor->toPlainText());
    const QString formatted = doc.toString(ui->spinBox->value());
    if(!formatted.isEmpty())
    {
        ui->codeEditor->setPlainText(formatted);
    }
}
