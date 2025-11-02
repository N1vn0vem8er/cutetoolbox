#include "csvdatatableinfo.h"
#include "config.h"
#include "src/widgets/ui_csvdatatableinfo.h"
#include <QFileDialog>
#include <QSettings>
#include <QStandardItemModel>

CSVDataTableInfo::CSVDataTableInfo(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::CSVDataTableInfo)
{
    ui->setupUi(this);
    setName(tr("Data Table Info"));
    connect(ui->openButton, &QPushButton::clicked, this, &CSVDataTableInfo::open);
    QSettings settings(Config::settingsName);
    int size = settings.beginReadArray("dataTableInfo.recentFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentFiles.append(path);
    }
    settings.endArray();
}

CSVDataTableInfo::~CSVDataTableInfo()
{
    QSettings settings(Config::settingsName);
    settings.beginWriteArray("dataTableInfo.recentFiles");
    for(int i = 0; i<recentFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool CSVDataTableInfo::canOpenFiles() const
{
    return true;
}

bool CSVDataTableInfo::canSaveFiles() const
{
    return true;
}

bool CSVDataTableInfo::canBasicEdit() const
{
    return true;
}

void CSVDataTableInfo::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(toCsv().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void CSVDataTableInfo::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.csv");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(toCsv().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void CSVDataTableInfo::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), "*.csv");
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        if(file.isOpen())
        {
            const QString csv = file.readAll();
            file.close();
            openedFile = path;
            parseCsv(csv);
            if(recentFiles.length() >= 10)
                recentFiles.removeFirst();
            if(!recentFiles.contains(openedFile))
                recentFiles.append(openedFile);
            emit updateRecent();
            emit opened(openedFile);
        }
    }
}

void CSVDataTableInfo::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

QString CSVDataTableInfo::getOpenedFileName() const
{
    return openedFile;
}

QStringList CSVDataTableInfo::getRecentFiles() const
{
    return recentFiles;
}

void CSVDataTableInfo::openFromRecent(const QString &path)
{
    if(recentFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            const QString csv = file.readAll();
            file.close();
            openedFile = path;
            parseCsv(csv);
            emit opened(openedFile);
        }
    }
}

void CSVDataTableInfo::clearRecent()
{
    recentFiles.clear();
    emit updateRecent();
}

void CSVDataTableInfo::parseCsv(const QString &csv)
{
    const QChar separator = ui->separatorLineEdit->text().isEmpty() ? ';' : ui->separatorLineEdit->text().at(0);
    const QStringList lines = csv.split('\n');
    if(lines.length() > 2)
    {
        QStandardItemModel* model = new QStandardItemModel(ui->table);
        if(ui->headerCheckBox->isChecked())
        {
            const QStringList header = lines.at(0).split(separator);
            for(int i = 0; i< header.length(); i++)
            {
                model->setHorizontalHeaderItem(i, new QStandardItem(header.at(i)));
            }
        }
        int maxSize = lines.length() > 1000 ? 1000 : lines.length();
        for(int i = lines.length() > 2 ? 1 : 0; i < maxSize; i++)
        {
            const QStringList row = lines.at(i).split(separator);
            QList<QStandardItem*> items;
            for(const auto& item : row)
            {
                items.append(new QStandardItem(item));
            }
            model->appendRow(items);
        }
        ui->table->setModel(model);
    }
}

QString CSVDataTableInfo::toCsv() const
{
    QString csv;
    return csv;
}
