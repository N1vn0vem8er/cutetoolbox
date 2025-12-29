#include "domtreewidget.h"
#include "src/widgets/ui_domtreewidget.h"

#include <QFileDialog>

DomTreeWidget::DomTreeWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::DomTreeWidget)
{
    ui->setupUi(this);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &DomTreeWidget::saveAs);
}

DomTreeWidget::~DomTreeWidget()
{
    delete ui;
}

bool DomTreeWidget::canOpenFiles() const
{
    return true;
}

bool DomTreeWidget::canSaveFiles() const
{
    return true;
}

bool DomTreeWidget::canBasicEdit() const
{
    return true;
}

bool DomTreeWidget::canChangeFont() const
{
    return true;
}

void DomTreeWidget::save()
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

void DomTreeWidget::saveAs()
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

void DomTreeWidget::open()
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

void DomTreeWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void DomTreeWidget::increaseFontSize()
{

}

void DomTreeWidget::decreaseFontSize()
{

}

void DomTreeWidget::setFontSize()
{

}

void DomTreeWidget::resetFontSize()
{

}

void DomTreeWidget::setFont()
{

}

QString DomTreeWidget::getOpenedFileName() const
{
    return openedFile;
}

QStringList DomTreeWidget::getRecentFiles() const
{
    return recentFiles;
}

void DomTreeWidget::openFromRecent(const QString &path)
{

}

void DomTreeWidget::clearRecent()
{

}
