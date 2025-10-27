#include "regexwidget.h"
#include "config.h"
#include "src/widgets/ui_regexwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QRegularExpression>
#include <qsettings.h>

RegexWidget::RegexWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RegexWidget)
{
    ui->setupUi(this);
    setName(tr("Regex Tester"));
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &RegexWidget::regex);
    connect(ui->plainTextEdit, &CodeEditor::textChanged, this, &RegexWidget::regex);
    connect(ui->openTextButton, &QPushButton::clicked, this, &RegexWidget::open);
    connect(ui->copyRegexButton, &QPushButton::clicked, this, [&]{ui->lineEdit->selectAll(); ui->lineEdit->copy();});
    connect(ui->pasteRegexButton, &QPushButton::clicked, ui->lineEdit, &QLineEdit::paste);
    connect(ui->copyTextButton, &QPushButton::clicked, ui->plainTextEdit, &CodeEditor::copyAll);
    connect(ui->pasteTextButton, &QPushButton::clicked, ui->plainTextEdit, &CodeEditor::paste);
    connect(ui->clearTextButton, &QPushButton::clicked, ui->plainTextEdit, &CodeEditor::clear);
    ui->plainTextEdit->setReplaceTabWithSpacesEnabled(false);
    ui->plainTextEdit->setAutoClosingEnabled(false);
    QSettings settings(Config::settingsName);
    int size = settings.beginReadArray("regexWidget.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

RegexWidget::~RegexWidget()
{
    QSettings settings(Config::settingsName);
    settings.beginWriteArray("regexWidget.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool RegexWidget::canOpenFiles() const
{
    return true;
}

bool RegexWidget::canBasicEdit() const
{
    return true;
}

bool RegexWidget::canChangeFont() const
{
    return true;
}

void RegexWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->plainTextEdit->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
            emit opened(openedFile);
        }
    }
}

void RegexWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void RegexWidget::increaseFontSize()
{
    ui->plainTextEdit->increaseFontSize();
}

void RegexWidget::decreaseFontSize()
{
    ui->plainTextEdit->decreaseFontSize();
}

void RegexWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->plainTextEdit->setFontSize(size);
}

void RegexWidget::resetFontSize()
{
    ui->plainTextEdit->setFontSize(10);
}

void RegexWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->plainTextEdit->setFont(font);
    }
}

QString RegexWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList RegexWidget::getRecentFiles() const
{
    return recentFiles;
}

void RegexWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->plainTextEdit->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void RegexWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
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
