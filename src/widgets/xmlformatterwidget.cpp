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
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &XMLFormatterWidget::open);
    int size = settings.beginReadArray("xmlFormatterWidget.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

XMLFormatterWidget::~XMLFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("xmlFormatterWidget.indentations", ui->spinBox->value());
    settings.beginWriteArray("xmlFormatterWidget.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

void XMLFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.xml");
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->codeEditor->setPlainText(file.readAll());
            openedFile = path;
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
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
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.xml");
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

QStringList XMLFormatterWidget::getRecentFiles() const
{
    return recentFiles;
}

void XMLFormatterWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void XMLFormatterWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
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
