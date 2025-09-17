#include "textprotoformatterwidget.h"
#include "src/widgets/ui_textprotoformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

TextProtoFormatterWidget::TextProtoFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::TextProtoFormatterWidget)
{
    ui->setupUi(this);
    QSettings settings("cutetoolbox");
    ui->styleComboBox->setCurrentIndex(settings.value("textprotoformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &TextProtoFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &TextProtoFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

TextProtoFormatterWidget::~TextProtoFormatterWidget()
{
    QSettings settings("cutetoolbox");
    settings.setValue("textprotoformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool TextProtoFormatterWidget::canOpenFiles() const
{
    return true;
}

bool TextProtoFormatterWidget::canSaveFiles() const
{
    return true;
}

bool TextProtoFormatterWidget::canBasicEdit() const
{
    return true;
}

bool TextProtoFormatterWidget::canChangeFont() const
{
    return true;
}

void TextProtoFormatterWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
        }
    }
    else
        saveAs();
}

void TextProtoFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.cs");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
        }
    }
}

void TextProtoFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.cs");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
        }
    }
}

void TextProtoFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void TextProtoFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void TextProtoFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void TextProtoFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void TextProtoFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

void TextProtoFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.textpb"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
