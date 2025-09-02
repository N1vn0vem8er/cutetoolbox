#include "textdiffwidget.h"
#include "src/widgets/ui_textdiffwidget.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <qfontdialog.h>
#include <qtextobject.h>

TextDiffWidget::TextDiffWidget(QWidget *parent)
    : CustomWidget(parent)
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

bool TextDiffWidget::canOpenFiles() const
{
    return true;
}

bool TextDiffWidget::canBasicEdit() const
{
    return true;
}

bool TextDiffWidget::canChangeFont() const
{
    return true;
}

void TextDiffWidget::open()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton oldTextButton(tr("Old Text"), &dialog);
    QPushButton newTextButton(tr("New Text"), &dialog);
    connect(&oldTextButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::oldText;});
    connect(&newTextButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::newText;});
    connect(&oldTextButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&newTextButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&oldTextButton);
    layout.addWidget(&newTextButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(option != TextEdits::none)
        {
            const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
            if(!path.isEmpty())
            {
                QFile file(path);
                file.open(QIODevice::ReadOnly);
                if(file.isOpen())
                {
                    if(option == newText)
                        ui->newText->setPlainText(file.readAll());
                    else if(option == oldText)
                        ui->oldText->setPlainText(file.readAll());
                    file.close();
                }
            }
        }
    }
}

void TextDiffWidget::increaseFontSize()
{
    if(ui->newText->hasFocus())
        ui->newText->increaseFontSize();
    else if(ui->oldText->hasFocus())
        ui->oldText->increaseFontSize();
    else if(ui->diff->hasFocus())
        ui->diff->increaseFontSize();
}

void TextDiffWidget::decreaseFontSize()
{
    if(ui->newText->hasFocus())
        ui->newText->decreaseFontSize();
    else if(ui->oldText->hasFocus())
        ui->oldText->decreaseFontSize();
    else if(ui->diff->hasFocus())
        ui->diff->decreaseFontSize();
}

void TextDiffWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
        if(ok)
        {
            switch(option)
            {
            case oldText:
                ui->oldText->setFontSize(size);
                break;
            case newText:
                ui->newText->setFontSize(size);
                break;
            case diff:
                ui->diff->setFontSize(size);
                break;
            default:
                break;
            }
        }
    }
}

void TextDiffWidget::resetFontSize()
{
    if(ui->newText->hasFocus())
        ui->newText->setFontSize(10);
    else if(ui->oldText->hasFocus())
        ui->oldText->setFontSize(10);
    else if(ui->diff->hasFocus())
        ui->diff->setFontSize(10);
}

void TextDiffWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            switch(option)
            {
            case oldText:
                ui->oldText->setFont(font);
                break;
            case newText:
                ui->newText->setFont(font);
                break;
            case diff:
                ui->diff->setFont(font);
                break;
            default:
                break;
            }
        }
    }
}

TextDiffWidget::TextEdits TextDiffWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton oldTextButton(tr("Old Text"), &dialog);
    QPushButton newTextButton(tr("New Text"), &dialog);
    QPushButton diffTextButton(tr("Diff"), &dialog);
    connect(&oldTextButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::oldText;});
    connect(&newTextButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::newText;});
    connect(&diffTextButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::diff;});
    connect(&oldTextButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&newTextButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&diffTextButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&oldTextButton);
    layout.addWidget(&newTextButton);
    layout.addWidget(&diffTextButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
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
