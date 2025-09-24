#include "xmlformatterwidget.h"
#include "src/widgets/ui_xmlformatterwidget.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QFontDialog>
#include <QInputDialog>
#include <QSettings>
#include "config.h"

XMLFormatterWidget::XMLFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::XMLFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("XML Formatter"));
    QSettings settings(Config::settingsName);
    ui->spinBox->setValue(settings.value("xmlFormatterWidget.indentations", 4).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &XMLFormatterWidget::format);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copy);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &XMLFormatterWidget::open);
}

XMLFormatterWidget::~XMLFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("xmlFormatterWidget.indentations", ui->spinBox->value());
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
            openedFile = path;
            file.close();
            emit opened(openedFile);
        }
    }
}

void XMLFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
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
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
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
            openedFile = path;
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
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

QString XMLFormatterWidget::getOpenedFileName() const
{
    return openedFile;
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
