#include "sqlformatterwidget.h"
#include "src/widgets/ui_sqlformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qregularexpression.h>

SQLFormatterWidget::SQLFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::SQLFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("SQL Formatter"));
    connect(ui->formatButton, &QPushButton::clicked, this, &SQLFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &SQLFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copy);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
}

SQLFormatterWidget::~SQLFormatterWidget()
{
    delete ui;
}

void SQLFormatterWidget::format()
{
    QString sql = ui->codeEditor->toPlainText();
    if(!sql.isEmpty())
    {
        const QStringList keywords = {"SELECT", "FROM", "WHERE", "ORDER", "GROUP", "BY", "JOIN", "LEFT", "RIGHT", "INNER", "OUTER", "ON", "AND", "OR"};
        for(const auto& keyword : keywords)
        {
            sql.replace(keyword, "\n" + keyword);
            sql.replace(keyword.toLower(), "\n" + keyword);
        }
        sql.replace(",", ",\n");
        ui->codeEditor->setPlainText(sql);
    }

}

void SQLFormatterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->codeEditor->setPlainText(file.readAll());
            openedFile = path;
            file.close();
        }
    }
}

bool SQLFormatterWidget::canOpenFiles() const
{
    return true;
}

bool SQLFormatterWidget::canSaveFiles() const
{
    return true;
}

bool SQLFormatterWidget::canBasicEdit() const
{
    return true;
}

bool SQLFormatterWidget::canChangeFont() const
{
    return true;
}

void SQLFormatterWidget::save()
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

void SQLFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            openedFile = path;
            file.close();
        }
    }
}

void SQLFormatterWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void SQLFormatterWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void SQLFormatterWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void SQLFormatterWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void SQLFormatterWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
}
