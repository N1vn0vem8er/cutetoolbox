#include "jsonformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_jsonformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <qsettings.h>

JsonFormatterWidget::JsonFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JsonFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("JSON Formatter"));
    QSettings settings(Config::settingsName);
    ui->spinBox->setValue(settings.value("jsonFormatterWidget.indentations", 4).toInt());
    syntaxHighlighter = new JSONSyntaxHighlighter(ui->codeEditor->document());
    connect(ui->formatButton, &QPushButton::clicked, this, &JsonFormatterWidget::format);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->openButton, &QPushButton::clicked, this, &JsonFormatterWidget::open);
}

JsonFormatterWidget::~JsonFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("jsonFormatterWidget.indentations", ui->spinBox->value());
    delete ui;
}

bool JsonFormatterWidget::canOpenFiles() const
{
    return true;
}

bool JsonFormatterWidget::canSaveFiles() const
{
    return true;
}

bool JsonFormatterWidget::canBasicEdit() const
{
    return true;
}

bool JsonFormatterWidget::canChangeFont() const
{
    return true;
}

void JsonFormatterWidget::save()
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

void JsonFormatterWidget::saveAs()
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

void JsonFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void JsonFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void JsonFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void JsonFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void JsonFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
}

QString JsonFormatterWidget::getOpenedFileName() const
{
    return openedFile;
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
            openedFile = path;
            file.close();
            emit opened(openedFile);
        }
    }
}

void JsonFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}
