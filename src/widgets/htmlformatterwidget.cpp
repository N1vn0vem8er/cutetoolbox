#include "htmlformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_htmlformatterwidget.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QFontDialog>
#include <QInputDialog>
#include <qsettings.h>

HTMLFormatterWidget::HTMLFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::HTMLFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("HTML Formatter"));
    QSettings settings(Config::settingsName);
    ui->spinBox->setValue(settings.value("htmlFormatterWidget.indentations", 4).toInt());
    syntaxHighlighter = new HTMLSyntaxHighlighter(ui->codeEditor->document());
    connect(ui->formatButton, &QPushButton::clicked, this, &HTMLFormatterWidget::format);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &HTMLFormatterWidget::open);
    int size = settings.beginReadArray("htmlFormatterWidget.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

HTMLFormatterWidget::~HTMLFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("htmlFormatterWidget.indentations", ui->spinBox->value());
    settings.beginWriteArray("htmlFormatterWidget.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
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
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void HTMLFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.html");
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

QString HTMLFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList HTMLFormatterWidget::getRecentFiles() const
{
    return recentFiles;
}

void HTMLFormatterWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void HTMLFormatterWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
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
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.html");
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit opened(openedFile);
            emit updateRecent();
        }
    }
}

void HTMLFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}
