#include "htmlformatterwidget.h"
#include "src/widgets/ui_htmlformatterwidget.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QFontDialog>
#include <QInputDialog>

HTMLFormatterWidget::HTMLFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::HTMLFormatterWidget)
{
    ui->setupUi(this);
    ui->spinBox->setValue(4);
    syntaxHighlighter = new HTMLSyntaxHighlighter(ui->codeEditor->document());
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

bool HTMLFormatterWidget::canOpenFiles() const
{
    return true;
}

bool HTMLFormatterWidget::canSaveFiles() const
{
    return true;
}

bool HTMLFormatterWidget::canBasicEdit() const
{
    return true;
}

bool HTMLFormatterWidget::canChangeFont() const
{
    return true;
}

void HTMLFormatterWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
        }
    }
    else
        saveAs();
}

void HTMLFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            openedFile = path;
            file.close();
        }
    }
}

void HTMLFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void HTMLFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void HTMLFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void HTMLFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void HTMLFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
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
            openedFile = path;
            file.close();
        }
    }
}
