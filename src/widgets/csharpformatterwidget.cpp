#include "csharpformatterwidget.h"
#include "config.h"
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
    setName(tr("C# Formatter"));
    QSettings settings(Config::settingsName);
    ui->styleComboBox->setCurrentIndex(settings.value("csformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &CSharpFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &CSharpFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    int size = settings.beginReadArray("csformatter.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

CSharpFormatterWidget::~CSharpFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("csformatter.style", ui->styleComboBox->currentIndex());
    settings.beginWriteArray("csformatter.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
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
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void CSharpFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.cs");
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

void CSharpFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.cs");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
            emit opened(openedFile);
        }
    }
}

void CSharpFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
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

QString CSharpFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList CSharpFormatterWidget::getRecentFiles() const
{
    return recentFiles;
}

void CSharpFormatterWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(path);
            file.close();
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void CSharpFormatterWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
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
