#include "javascriptformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_javascriptformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

JavaScriptFormatterWidget::JavaScriptFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JavaScriptFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("JavaScript Formatter"));
    QSettings settings(Config::settingsName);
    ui->styleComboBox->setCurrentIndex(settings.value("javascriptformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &JavaScriptFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &JavaScriptFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

JavaScriptFormatterWidget::~JavaScriptFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("javascriptformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool JavaScriptFormatterWidget::canOpenFiles() const
{
    return true;
}

bool JavaScriptFormatterWidget::canSaveFiles() const
{
    return true;
}

bool JavaScriptFormatterWidget::canBasicEdit() const
{
    return true;
}

bool JavaScriptFormatterWidget::canChangeFont() const
{
    return true;
}

void JavaScriptFormatterWidget::save()
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

void JavaScriptFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.js *.mjs *.cjs *.ts");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void JavaScriptFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.js *.mjs *.cjs *.ts");
    if(!path.isEmpty())
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

void JavaScriptFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void JavaScriptFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void JavaScriptFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void JavaScriptFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void JavaScriptFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void JavaScriptFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

QString JavaScriptFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

void JavaScriptFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.js"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
