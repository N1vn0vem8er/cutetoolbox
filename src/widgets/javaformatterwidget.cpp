#include "javaformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_javaformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QProcess>
#include <qsettings.h>

JavaFormatterWidget::JavaFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JavaFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("Java Formatter"));
    QSettings settings(Config::settingsName);
    ui->styleComboBox->setCurrentIndex(settings.value("javaformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &JavaFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &JavaFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

JavaFormatterWidget::~JavaFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("javaformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool JavaFormatterWidget::canOpenFiles() const
{
    return true;
}

bool JavaFormatterWidget::canSaveFiles() const
{
    return true;
}

bool JavaFormatterWidget::canBasicEdit() const
{
    return true;
}

bool JavaFormatterWidget::canChangeFont() const
{
    return true;
}

void JavaFormatterWidget::save()
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

void JavaFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.java");
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

void JavaFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.java");
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

void JavaFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void JavaFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void JavaFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void JavaFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void JavaFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void JavaFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

QString JavaFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

void JavaFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.java"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
