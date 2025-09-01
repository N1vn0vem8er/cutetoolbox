#include "xmlformatterwidget.h"
#include "src/widgets/ui_xmlformatterwidget.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QFontDialog>
#include <QInputDialog>

XMLFormatterWidget::XMLFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
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

bool XMLFormatterWidget::canOpenFiles() const
{
    return true;
}

bool XMLFormatterWidget::canSaveFiles() const
{
    return true;
}

bool XMLFormatterWidget::canBasicEdit() const
{
    return true;
}

bool XMLFormatterWidget::canChangeFont() const
{
    return true;
}

void XMLFormatterWidget::save()
{

}

void XMLFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
        }
    }
}

void XMLFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void XMLFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void XMLFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void XMLFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void XMLFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
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
