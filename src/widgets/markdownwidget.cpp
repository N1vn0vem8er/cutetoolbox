#include "markdownwidget.h"
#include "config.h"
#include "previewpage.h"
#include "src/widgets/ui_markdownwidget.h"
#include "syntaxhighlighters/markdownsyntaxhighlighter.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QWebChannel>
#include <qdir.h>
#include <qsettings.h>

MarkdownWidget::MarkdownWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::MarkdownWidget)
{
    ui->setupUi(this);
    setName(tr("Markdown Preview"));
    connect(ui->openButton, &QPushButton::clicked, this, &MarkdownWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->editor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->editor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->editor, &CodeEditor::clear);
    connect(ui->openedFileButton, &QPushButton::clicked, this, &MarkdownWidget::showOpenedFile);
    connect(ui->previousButton, &QPushButton::clicked, this, &MarkdownWidget::previous);
    connect(ui->nextButton, &QPushButton::clicked, this, &MarkdownWidget::next);
    syntaxHighlighter = new MarkdownSyntaxHighlighter(ui->editor->document());
    ui->preview->setContextMenuPolicy(Qt::NoContextMenu);
    page = new PreviewPage(this);
    connect(page, &PreviewPage::openedFile, this, &MarkdownWidget::openFileInPreview);
    ui->preview->setPage(page);
    connect(ui->editor, &CodeEditor::textChanged, this, [this]{
        page->setAbsolutePath(openedFile);
        document.setText(ui->editor->toPlainText());
    });
    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &document);
    page->setWebChannel(channel);
    ui->preview->setUrl(QUrl("qrc:/index.html"));
    QSettings settings(Config::settingsName);
    int size = settings.beginReadArray("markdownWidget.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

MarkdownWidget::~MarkdownWidget()
{
    QSettings settings(Config::settingsName);
    settings.beginWriteArray("markdownWidget.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool MarkdownWidget::canOpenFiles() const
{
    return true;
}

bool MarkdownWidget::canSaveFiles() const
{
    return true;
}

bool MarkdownWidget::canBasicEdit() const
{
    return true;
}

bool MarkdownWidget::canChangeFont() const
{
    return true;
}

void MarkdownWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->editor->toPlainText().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void MarkdownWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.md");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->editor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            page->setAbsolutePath(openedFile);
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void MarkdownWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.md");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->editor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            page->setAbsolutePath(openedFile);
            addPrevious(openedFile);
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
            emit opened(openedFile);
        }
    }
}

void MarkdownWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void MarkdownWidget::increaseFontSize()
{
    ui->editor->increaseFontSize();
}

void MarkdownWidget::decreaseFontSize()
{
    ui->editor->decreaseFontSize();
}

void MarkdownWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->editor->setFontSize(size);
}

void MarkdownWidget::resetFontSize()
{
    ui->editor->setFontSize(10);
}

void MarkdownWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->editor->setFont(font);
}

QString MarkdownWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList MarkdownWidget::getRecentFiles() const
{
    return recentFiles;
}

void MarkdownWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->editor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void MarkdownWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}

void MarkdownWidget::addPrevious(const QString &path)
{
    if(previousStack.length() > 0 && previousStack.top() == path)
        return;
    previousStack.push(path);
}

void MarkdownWidget::addNext(const QString &path)
{
    if(nextStack.length() > 0 && nextStack.top() == path)
        return;
    nextStack.push(path);
}

void MarkdownWidget::openFileInPreview(const QString &path)
{
    addPrevious(path);
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        document.setText(file.readAll());
        page->setAbsolutePath(path);
        file.close();
    }
}

void MarkdownWidget::showOpenedFile()
{
    document.setText(ui->editor->toPlainText());
    page->setAbsolutePath(openedFile);
    addPrevious(openedFile);
}

void MarkdownWidget::previous()
{
    if(previousStack.length() > 1)
    {
        addNext(previousStack.pop());
        if(previousStack.length() > 0)
        {
            QFile file(previousStack.top());
            if(file.open(QIODevice::ReadOnly))
            {
                document.setText(file.readAll());
                page->setAbsolutePath(previousStack.top());
                file.close();
            }
        }
    }
}

void MarkdownWidget::next()
{
    if(nextStack.length() > 0)
    {
        addPrevious(nextStack.top());
        if(nextStack.length() > 0)
        {
            QFile file(nextStack.top());
            if(file.open(QIODevice::ReadOnly))
            {
                document.setText(file.readAll());
                page->setAbsolutePath(nextStack.pop());
                file.close();
            }
        }
    }
}
