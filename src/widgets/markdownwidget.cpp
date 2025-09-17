#include "markdownwidget.h"
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

MarkdownWidget::MarkdownWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::MarkdownWidget)
{
    ui->setupUi(this);
    setName(tr("Markdown Preview"));
    syntaxHighlighter = new MarkdownSyntaxHighlighter(ui->editor->document());
    ui->preview->setContextMenuPolicy(Qt::NoContextMenu);
    PreviewPage* page = new PreviewPage(this);
    ui->preview->setPage(page);
    connect(ui->editor, &CodeEditor::textChanged, this, [this]{document.setText(ui->editor->toPlainText());});
    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &document);
    page->setWebChannel(channel);
    ui->preview->setUrl(QUrl("qrc:/index.html"));
}

MarkdownWidget::~MarkdownWidget()
{
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
        }
    }
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
