#include "csharpformatterwidget.h"
#include "src/widget/ui_csharpformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

CSharpFormatterWidget::CSharpFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CSharpFormatterWidget)
{
    ui->setupUi(this);
    QSettings settings("cutetoolbox");
    ui->styleComboBox->setCurrentIndex(settings.value("csformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &CSharpFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &CSharpFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

CSharpFormatterWidget::~CSharpFormatterWidget()
{
    QSettings settings("cutetoolbox");
    settings.setValue("csformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool CSharpFormatterWidget::canOpenFiles() const
{
    return true;
}

bool CSharpFormatterWidget::canSaveFiles() const
{
    return true;
}

bool CSharpFormatterWidget::canBasicEdit() const
{
    return true;
}

bool CSharpFormatterWidget::canChangeFont() const
{
    return true;
}

void CSharpFormatterWidget::save()
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

void CSharpFormatterWidget::saveAs()
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

void CSharpFormatterWidget::open()
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

void CSharpFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void CSharpFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void CSharpFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void CSharpFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void CSharpFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

void CSharpFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.cs"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
