#include "cppformatterwidget.h"
#include "src/widget/ui_cppformatterwidget.h"

#include <QProcess>
#include <qsettings.h>

CppFormatterWidget::CppFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CppFormatterWidget)
{
    ui->setupUi(this);
    QSettings settings("cutetoolbox");
    ui->styleComboBox->setCurrentIndex(settings.value("cppformatter.style", 0).toInt());
    connect(ui->formatButton, &QPushButton::clicked, this, &CppFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &CppFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

CppFormatterWidget::~CppFormatterWidget()
{
    QSettings settings("cutetoolbox");
    settings.setValue("cppformatter.style", ui->styleComboBox->currentIndex());
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

}

void CppFormatterWidget::saveAs()
{

}

void CppFormatterWidget::open()
{

}

void CppFormatterWidget::increaseFontSize()
{

}

void CppFormatterWidget::decreaseFontSize()
{

}

void CppFormatterWidget::setFontSize()
{

}

void CppFormatterWidget::resetFontSize()
{

}

void CppFormatterWidget::setFont()
{

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
