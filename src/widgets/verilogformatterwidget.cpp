#include "verilogformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_verilogformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

VerilogFormatterWidget::VerilogFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::VerilogFormatterWidget)
{
    ui->setupUi(this);
    QSettings settings(Config::settingsName);
    setName(tr("Verilog Formatter"));
    ui->styleComboBox->setCurrentIndex(settings.value("verilogformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &VerilogFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &VerilogFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

VerilogFormatterWidget::~VerilogFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("verilogformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool VerilogFormatterWidget::canOpenFiles() const
{
    return true;
}

bool VerilogFormatterWidget::canSaveFiles() const
{
    return true;
}

bool VerilogFormatterWidget::canBasicEdit() const
{
    return true;
}

bool VerilogFormatterWidget::canChangeFont() const
{
    return true;
}

void VerilogFormatterWidget::save()
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

void VerilogFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.sv *.svh *.v *.vh");
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

void VerilogFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.sv *.svh *.v *.vh");
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

void VerilogFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void VerilogFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void VerilogFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void VerilogFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void VerilogFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

QString VerilogFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

void VerilogFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.sv"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
