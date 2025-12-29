#include "domtreewidget.h"
#include "src/widgets/ui_domtreewidget.h"
#include <QDomDocument>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qstandarditemmodel.h>

DomTreeWidget::DomTreeWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::DomTreeWidget)
{
    ui->setupUi(this);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &DomTreeWidget::saveAs);
    connect(ui->codeEditor, &CodeEditor::textChanged, this, &DomTreeWidget::generate);
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
    ui->codeEditor->increaseFontSize();
}

void DomTreeWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void DomTreeWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void DomTreeWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void DomTreeWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
        ui->codeEditor->setFont(font);
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

void DomTreeWidget::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}

void DomTreeWidget::toStandardItem(const QDomNode &xmlNode, QStandardItem *parentItem)
{
    QDomNode node = xmlNode.firstChild();
    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            QStandardItem* item = new QStandardItem(element.tagName());
            if(!element.text().isEmpty() && element.firstChild().isText())
            {
                item->appendRow(new QStandardItem(element.text()));
            }
            parentItem->appendRow(item);
            toStandardItem(node, item);
        }
        node = node.nextSibling();
    }
}

void DomTreeWidget::generate()
{
    if(ui->treeView->model())
        ui->treeView->model()->deleteLater();
    QDomDocument doc;
    doc.setContent(ui->codeEditor->toPlainText());
    QDomElement root = doc.documentElement();
    QStandardItemModel* model = new QStandardItemModel();
    QStandardItem* rootItem = new QStandardItem(root.tagName());
    model->appendRow(rootItem);
    toStandardItem(root, rootItem);
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
}
