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
    ui->headerCheckBox->setChecked(settings.value("dataTableInfo.header", true).toBool());
    ui->separatorLineEdit->setText(settings.value("dataTableInfo.separator", ";").toString());
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
    settings.setValue("dataTableInfo.header", ui->headerCheckBox->isChecked());
    settings.setValue("dataTableInfo.separator", ui->separatorLineEdit->text());
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
    const QStringList lines = csv.split('\n', Qt::KeepEmptyParts);
    if(lines.length() > 1)
    {
        QStandardItemModel* model = new QStandardItemModel(ui->table);
        QStandardItemModel* infoModel = new QStandardItemModel(ui->columnsInfoTable);
        infoModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Column Name")));
        infoModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Missing Values")));
        QMap<int, int> emptyRows;
        int missingValues = 0;
        const QStringList header = lines.at(0).split(separator, Qt::KeepEmptyParts);
        if(ui->headerCheckBox->isChecked())
        {
            for(int i = 0; i< header.length(); i++)
            {
                model->setHorizontalHeaderItem(i, new QStandardItem(header.at(i)));
                emptyRows[i] = 0;
            }
            ui->columnsLabel->setText(tr("Columns: %1").arg(header.length()));
        }
        int maxSize = lines.length() > 1000 ? 1000 : lines.length();
        for(int i = ui->headerCheckBox->isChecked() ? 1 : 0; i < maxSize; i++)
        {
            const QStringList row = lines.at(i).split(separator, Qt::KeepEmptyParts);
            QList<QStandardItem*> items;
            for(int item = 0; item < row.length(); item++)
            {
                items.append(new QStandardItem(row.at(item)));
            }
            model->appendRow(items);
        }
        for(int i = ui->headerCheckBox->isChecked() ? 1 : 0; i < lines.length(); i++)
        {
            const QStringList row = lines.at(i).split(separator, Qt::KeepEmptyParts);
            for(int item = 0; item < row.length(); item++)
            {
                if(row.at(item).isEmpty())
                {
                    emptyRows[item]++;
                    missingValues++;
                }
            }
        }
        if(ui->headerCheckBox->isChecked())
            for(int i=0;i<header.size(); i++)
            {
                QList<QStandardItem*> items;
                items.append(new QStandardItem(header.at(i)));
                items.append(new QStandardItem(QString::number(emptyRows.value(i))));
                infoModel->appendRow(items);
            }
        ui->columnsInfoTable->setModel(infoModel);
        ui->table->setModel(model);
        ui->missingValuesLabel->setText(tr("Missing Values: %1").arg(missingValues));
        ui->rowsLabel->setText(tr("Rows: %1").arg(ui->headerCheckBox->isChecked() ? lines.length() - 1 : lines.length()));
    }
}

QString CSVDataTableInfo::toCsv() const
{
    QString csv;
    return csv;
}
