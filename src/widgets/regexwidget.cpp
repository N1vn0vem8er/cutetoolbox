#include "regexwidget.h"
#include "src/widgets/ui_regexwidget.h"
#include <QFileDialog>
#include <QRegularExpression>

RegexWidget::RegexWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegexWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &RegexWidget::regex);
    connect(ui->plainTextEdit, &CodeEditor::textChanged, this, &RegexWidget::regex);
    connect(ui->openTextButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            file.open(QIODevice::ReadOnly);
            if(file.isOpen())
            {
                ui->plainTextEdit->setPlainText(file.readAll());
                file.close();
            }
        }
    });
    connect(ui->copyRegexButton, &QPushButton::clicked, this, [&]{ui->lineEdit->selectAll(); ui->lineEdit->copy();});
    connect(ui->pasteRegexButton, &QPushButton::clicked, ui->lineEdit, &QLineEdit::paste);
    connect(ui->clearRegexButton, &QPushButton::clicked, ui->lineEdit, &QLineEdit::clear);
    connect(ui->copyTextButton, &QPushButton::clicked, ui->plainTextEdit, &CodeEditor::copyAll);
    connect(ui->pasteTextButton, &QPushButton::clicked, ui->plainTextEdit, &CodeEditor::paste);
    connect(ui->clearTextButton, &QPushButton::clicked, ui->plainTextEdit, &CodeEditor::clear);
}

RegexWidget::~RegexWidget()
{
    delete ui;
}

void RegexWidget::regex()
{
    QList<QTextEdit::ExtraSelection> found;
    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::darkBlue);
    QRegularExpression regex = QRegularExpression(ui->lineEdit->text());
    if(regex.isValid())
    {
        ui->errorLabel->clear();
        ui->lineEdit->setPalette(QApplication::palette(ui->lineEdit));
        QRegularExpressionMatchIterator i = regex.globalMatch(ui->plainTextEdit->toPlainText());
        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QTextEdit::ExtraSelection es;
            es.format = highlightFormat;
            es.cursor = ui->plainTextEdit->textCursor();
            es.cursor.setPosition(match.capturedStart());
            es.cursor.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
            found.append(es);
        }
        ui->plainTextEdit->setExtraSelections(found);
    }
    else
    {
        ui->errorLabel->setText(regex.errorString());
        QPalette palette = ui->lineEdit->palette();
        palette.setColor(QPalette::Base, Qt::red);
        palette.setColor(QPalette::Text, Qt::white);
        ui->lineEdit->setPalette(palette);
    }
}
