#include "cppformatterwidget.h"
#include "config.h"
#include "src/widget/ui_cppformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QProcess>
#include <qsettings.h>

CppFormatterWidget::CppFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CppFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("C++ Formatter"));
    QSettings settings(Config::settingsName);
    ui->styleComboBox->setCurrentIndex(settings.value("cppformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &CppFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &CppFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);

    int size = settings.beginReadArray("cppformatter.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

CppFormatterWidget::~CppFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("cppformatter.style", ui->styleComboBox->currentIndex());
    settings.beginWriteArray("cppformatter.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool CppFormatterWidget::canOpenFiles() const
{
    return true;
}

bool CppFormatterWidget::canSaveFiles() const
{
    return true;
}

bool CppFormatterWidget::canBasicEdit() const
{
    return true;
}

bool CppFormatterWidget::canChangeFont() const
{
    return true;
}

void CppFormatterWidget::save()
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

void CppFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.cpp *.h *.hpp");
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

void CppFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.cpp *.h *.hpp");
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

void CppFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void CppFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void CppFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void CppFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void CppFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void CppFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

QString CppFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList CppFormatterWidget::getRecentFiles() const
{
    return recentFiles;
}

void CppFormatterWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
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

void CppFormatterWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}

void CppFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.cpp"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
