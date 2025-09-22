#include "objectivecformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_objectivecformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

ObjectiveCFormatterWidget::ObjectiveCFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ObjectiveCFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("Objective C Formatter"));
    QSettings settings(Config::settingsName);
    ui->styleComboBox->setCurrentIndex(settings.value("objectivecformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &ObjectiveCFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &ObjectiveCFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

ObjectiveCFormatterWidget::~ObjectiveCFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("objectivecformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool ObjectiveCFormatterWidget::canOpenFiles() const
{
    return true;
}

bool ObjectiveCFormatterWidget::canSaveFiles() const
{
    return true;
}

bool ObjectiveCFormatterWidget::canBasicEdit() const
{
    return true;
}

bool ObjectiveCFormatterWidget::canChangeFont() const
{
    return true;
}

void ObjectiveCFormatterWidget::save()
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

void ObjectiveCFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.m *.mm");
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

void ObjectiveCFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.m *.mm");
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

void ObjectiveCFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void ObjectiveCFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void ObjectiveCFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void ObjectiveCFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void ObjectiveCFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

QString ObjectiveCFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

void ObjectiveCFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.m"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
