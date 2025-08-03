#include "sqlformatterwidget.h"
#include "src/widgets/ui_sqlformatterwidget.h"
#include <QFileDialog>
#include <qregularexpression.h>

SQLFormatterWidget::SQLFormatterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SQLFormatterWidget)
{
    ui->setupUi(this);
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
            file.close();
        }
    }
}
