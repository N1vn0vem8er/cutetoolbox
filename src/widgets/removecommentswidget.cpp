#include "removecommentswidget.h"
#include "src/widgets/ui_removecommentswidget.h"

#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <qinputdialog.h>

RemoveCommentsWidget::RemoveCommentsWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RemoveCommentsWidget)
{
    ui->setupUi(this);
}

RemoveCommentsWidget::~RemoveCommentsWidget()
{
    delete ui;
}

bool RemoveCommentsWidget::canOpenFiles() const
{
    return true;
}

bool RemoveCommentsWidget::canSaveFiles() const
{
    return true;
}

bool RemoveCommentsWidget::canBasicEdit() const
{
    return true;
}

bool RemoveCommentsWidget::canChangeFont() const
{
    return true;
}

void RemoveCommentsWidget::save()
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

void RemoveCommentsWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void RemoveCommentsWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
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

void RemoveCommentsWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void RemoveCommentsWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void RemoveCommentsWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void RemoveCommentsWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void RemoveCommentsWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void RemoveCommentsWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
}

QString RemoveCommentsWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList RemoveCommentsWidget::getRecentFiles() const
{
    return recentFiles;
}

void RemoveCommentsWidget::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->codeEditor->setPlainText(file.readAll());
            file.close();
            openedFile = path;
            emit opened(openedFile);
        }
    }
}

void RemoveCommentsWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}
