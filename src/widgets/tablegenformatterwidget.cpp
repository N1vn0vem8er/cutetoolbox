#include "tablegenformatterwidget.h"
#include "src/widgets/ui_tablegenformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qprocess.h>
#include <qsettings.h>

TableGenFormatterWidget::TableGenFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::TableGenFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("TableGen Formatter"));
    QSettings settings("cutetoolbox");
    ui->styleComboBox->setCurrentIndex(settings.value("tabelgenformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &TableGenFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &TableGenFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

TableGenFormatterWidget::~TableGenFormatterWidget()
{
    QSettings settings("cutetoolbox");
    settings.setValue("tabelgenformatter.style", ui->styleComboBox->currentIndex());
    delete ui;
}

bool TableGenFormatterWidget::canOpenFiles() const
{
    return true;
}

bool TableGenFormatterWidget::canSaveFiles() const
{
    return true;
}

bool TableGenFormatterWidget::canBasicEdit() const
{
    return true;
}

bool TableGenFormatterWidget::canChangeFont() const
{
    return true;
}

void TableGenFormatterWidget::save()
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

void TableGenFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.td");
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

void TableGenFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.td");
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

void TableGenFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void TableGenFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void TableGenFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void TableGenFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void TableGenFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
}

void TableGenFormatterWidget::format()
{
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{ui->codeEditor->setPlainText(process->readAllStandardOutput());});
    connect(process, &QProcess::readyReadStandardError, this, [this, process]{qDebug() << process->readAllStandardError();});
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    process->start("clang-format", {QString("--style=%1").arg(ui->styleComboBox->currentText()), "--assume-filename=some_file.td"});
    process->waitForStarted();
    process->write(ui->codeEditor->toPlainText().toUtf8());
    process->closeWriteChannel();
}
