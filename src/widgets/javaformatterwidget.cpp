#include "javaformatterwidget.h"
#include "src/widgets/ui_javaformatterwidget.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QProcess>

JavaFormatterWidget::JavaFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JavaFormatterWidget)
{
    ui->setupUi(this);
}

JavaFormatterWidget::~JavaFormatterWidget()
{
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
        }
    }
    else
        saveAs();
}

void JavaFormatterWidget::saveAs()
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

void JavaFormatterWidget::open()
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
