#include "markdownwidget.h"
#include "previewpage.h"
#include "src/widgets/ui_markdownwidget.h"

#include <QWebChannel>

MarkdownWidget::MarkdownWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MarkdownWidget)
{
    ui->setupUi(this);
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

void MarkdownWidget::displayDocument()
{

}
