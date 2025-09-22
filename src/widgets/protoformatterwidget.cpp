#include "protoformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_protoformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

ProtoFormatterWidget::ProtoFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ProtoFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("Proto Formatter"));
    QSettings settings(Config::settingsName);
    ui->styleComboBox->setCurrentIndex(settings.value("protoformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &ProtoFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &ProtoFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

ProtoFormatterWidget::~ProtoFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("protoformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool ProtoFormatterWidget::canOpenFiles() const
{
    return true;
}

bool ProtoFormatterWidget::canSaveFiles() const
{
    return true;
}

bool ProtoFormatterWidget::canBasicEdit() const
{
    return true;
}

bool ProtoFormatterWidget::canChangeFont() const
{
    return true;
}

void ProtoFormatterWidget::save()
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

void ProtoFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.proto *.protodevel");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
}

void ProtoFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.proto *.protodevel");
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

void ProtoFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void ProtoFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void ProtoFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void ProtoFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void ProtoFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

QString ProtoFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

void ProtoFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.proto"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}

