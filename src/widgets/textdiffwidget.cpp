#include "textdiffwidget.h"
#include "src/widgets/ui_textdiffwidget.h"
#include <QFileDialog>
#include <QTextStream>
#include <qtextobject.h>

TextDiffWidget::TextDiffWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TextDiffWidget)
{
    ui->setupUi(this);
    connect(ui->oldText, &CodeEditor::textChanged, this, &TextDiffWidget::showDiff);
    connect(ui->newText, &CodeEditor::textChanged, this, &TextDiffWidget::showDiff);
    connect(ui->pasteOldButton, &QPushButton::clicked, ui->oldText, &CodeEditor::paste);
    connect(ui->newPasteButton, &QPushButton::clicked, ui->newText, &CodeEditor::paste);
    connect(ui->copyOldButton, &QPushButton::clicked, ui->oldText, &CodeEditor::copyAll);
    connect(ui->newCopyButton, &QPushButton::clicked, ui->newText, &CodeEditor::copyAll);
    connect(ui->clearOldButton, &QPushButton::clicked, ui->oldText, &CodeEditor::clear);
    connect(ui->newClearButton, &QPushButton::clicked, ui->newText, &CodeEditor::clear);
    connect(ui->openOldButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->oldText->setPlainText(file.readAll());
                file.close();
            }
        }
    });
    connect(ui->newOpenButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->newText->setPlainText(file.readAll());
                file.close();
            }
        }
    });
    connect(ui->copyDiffButton, &QPushButton::clicked, ui->diff, [&]{ui->diff->selectAll(); ui->diff->copy();});
}

TextDiffWidget::~TextDiffWidget()
{
    delete ui;
}

void TextDiffWidget::showDiff()
{
    const QStringList oldText = ui->oldText->toPlainText().split("\n");
    const QStringList newText = ui->newText->toPlainText().split("\n");
    QSet<QString> set1(oldText.begin(), oldText.end());
    QSet<QString> set2(newText.begin(), newText.end());
    QSet<QString> removedLines = set1;
    removedLines.subtract(set2);
    QSet<QString> addedLines = set2;
    addedLines.subtract(set1);

    QList<QTextEdit::ExtraSelection> extraSelections;
    ui->diff->clear();
    for(const QString& line : oldText)
    {
        ui->diff->appendPlainText(line);
        if(removedLines.contains(line))
        {
            QTextEdit::ExtraSelection selection;
            QTextCharFormat format = selection.format;
            format.setBackground(Qt::red);
            selection.format = format;
            selection.cursor = ui->diff->textCursor();
            selection.cursor.select(QTextCursor::LineUnderCursor);
            extraSelections.append(selection);
        }
    }
    for(const QString& line : newText)
    {
        if(addedLines.contains(line))
        {
            ui->diff->appendPlainText(line);
            QTextEdit::ExtraSelection selection;
            QTextCharFormat format = selection.format;
            format.setBackground(Qt::green);
            selection.format = format;
            selection.cursor = ui->diff->textCursor();
            selection.cursor.select(QTextCursor::LineUnderCursor);
            extraSelections.append(selection);
        }
    }
    ui->diff->setExtraSelections(extraSelections);
}
