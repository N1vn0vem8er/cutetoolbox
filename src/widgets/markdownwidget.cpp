#include "markdownwidget.h"
#include "previewpage.h"
#include "src/widgets/ui_markdownwidget.h"
#include "syntaxhighlighters/markdownsyntaxhighlighter.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QWebChannel>
#include <qdir.h>

MarkdownWidget::MarkdownWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MarkdownWidget)
{
    ui->setupUi(this);
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
    if(syntaxHighlighter)
        syntaxHighlighter->deleteLater();
    delete ui;
}
