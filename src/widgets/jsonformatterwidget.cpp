#include "jsonformatterwidget.h"
#include "src/widgets/ui_jsonformatterwidget.h"

#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>

JsonFormatterWidget::JsonFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JsonFormatterWidget)
{
    ui->setupUi(this);
    syntaxHighlighter = new JSONSyntaxHighlighter(ui->codeEditor->document());
    connect(ui->formatButton, &QPushButton::clicked, this, &JsonFormatterWidget::format);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &JsonFormatterWidget::open);
}

JsonFormatterWidget::~JsonFormatterWidget()
{
    delete ui;
}

void JsonFormatterWidget::format()
{
    QJsonParseError parseError;
    const QJsonDocument json = QJsonDocument::fromJson(ui->codeEditor->toPlainText().toLatin1(), &parseError);
    const QString formatted = json.toJson();
    if(!formatted.isEmpty() && parseError.error == QJsonParseError::NoError)
    {
        ui->codeEditor->setPlainText(formatted);
        ui->errorLabel->clear();
    }
    else
        ui->errorLabel->setText(parseError.errorString());
}

void JsonFormatterWidget::open()
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
