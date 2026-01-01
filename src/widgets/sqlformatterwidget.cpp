#include "sqlformatterwidget.h"
#include "config.h"
#include "src/widgets/ui_sqlformatterwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qregularexpression.h>
#include <qsettings.h>

SQLFormatterWidget::SQLFormatterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::SQLFormatterWidget)
{
    ui->setupUi(this);
    setName(tr("SQL Formatter"));
    connect(ui->formatButton, &QPushButton::clicked, this, &SQLFormatterWidget::format);
    connect(ui->openButton, &QPushButton::clicked, this, &SQLFormatterWidget::open);
    connect(ui->copyButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::copyAll);
    connect(ui->pasteButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::paste);
    connect(ui->clearButton, &QPushButton::clicked, ui->codeEditor, &CodeEditor::clear);
    QSettings settings(Config::settingsName);
    int size = settings.beginReadArray("sqlFormatter.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

SQLFormatterWidget::~SQLFormatterWidget()
{
    QSettings settings(Config::settingsName);
    settings.beginWriteArray("sqlFormatter.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
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
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.sql");
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            ui->codeEditor->setPlainText(file.readAll());
            openedFile = path;
            file.close();
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
            emit opened(openedFile);
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
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void SQLFormatterWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath(), "*.sql");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            openedFile = path;
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void SQLFormatterWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
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

QString SQLFormatterWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList SQLFormatterWidget::getRecentFiles() const
{
    return recentFiles;
}

void SQLFormatterWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void SQLFormatterWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}
