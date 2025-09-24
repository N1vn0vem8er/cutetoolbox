#include "usergeneratorwidget.h"
#include "config.h"
#include "src/widgets/ui_usergeneratorwidget.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <qdialog.h>
#include <random>
#include <QClipboard>
#include <QSettings>

UserGeneratorWidget::UserGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::UserGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("User Generator"));
    QSettings settings(Config::settingsName);
    ui->phoneLength->setValue(settings.value("userGenerator.phoneLength", 10).toInt());
    ui->quantitySpinBox->setValue(settings.value("userGenerator.quantity", 10).toInt());
    ui->firstNameCheckBox->setChecked(settings.value("userGenerator.firstName", true).toBool());
    ui->lastNameCheckBox->setChecked(settings.value("userGenerator.lastName", true).toBool());
    ui->emailCheckBox->setChecked(settings.value("userGenerator.email", true).toBool());
    ui->usernameCheckBox->setChecked(settings.value("userGenerator.username", true).toBool());
    ui->phoneNumberCheckBox->setChecked(settings.value("userGenerator.phoneNumber", true).toBool());
    connect(ui->generateButton, &QPushButton::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->quantitySpinBox, &QSpinBox::valueChanged, this, &UserGeneratorWidget::generate);
    connect(ui->phoneLength, &QSpinBox::valueChanged, this, &UserGeneratorWidget::generate);
    connect(ui->firstNameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->lastNameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->emailCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->usernameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->phoneNumberCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{
        QGuiApplication::clipboard()->setText(toCsv());
    });
    connect(ui->saveAsButton, &QPushButton::clicked, this, &UserGeneratorWidget::saveAs);
    generate();
}

UserGeneratorWidget::~UserGeneratorWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("userGenerator.phoneLength", ui->phoneLength->value());
    settings.setValue("userGenerator.quantity", ui->quantitySpinBox->value());
    settings.setValue("userGenerator.firstName", ui->firstNameCheckBox->isChecked());
    settings.setValue("userGenerator.lastName", ui->lastNameCheckBox->isChecked());
    settings.setValue("userGenerator.email", ui->emailCheckBox->isChecked());
    settings.setValue("userGenerator.username", ui->usernameCheckBox->isChecked());
    settings.setValue("userGenerator.phoneNumber", ui->phoneNumberCheckBox->isChecked());
    delete ui;
}

bool UserGeneratorWidget::canSaveFiles() const
{
    return true;
}

bool UserGeneratorWidget::canBasicEdit() const
{
    return true;
}

void UserGeneratorWidget::save()
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

void UserGeneratorWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {

        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(toCsv().toUtf8());
            openedFile = path;
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void UserGeneratorWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

QString UserGeneratorWidget::getOpenedFileName() const
{
    return openedFile;
}

QString UserGeneratorWidget::getRandomQString(const QStringList &list) const
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(0, list.length() - 1);
    return list[random(generator)];
}

QString UserGeneratorWidget::toCsv() const
{
    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->tableView->model());
    QString out;
    for(int col = 0; col < model->columnCount(); col++)
    {
        QString header = model->horizontalHeaderItem(col)->text();
        header.replace("\"", "\"\"");
        out += "\"" + header + "\"" + ",";
    }
    out += "\n";
    for(int row = 0; row < model->rowCount(); row++)
    {
        for(int col = 0; col < model->columnCount(); col++)
        {
            QStandardItem* item = model->item(row, col);
            if(item)
            {
                QString text = item->text();
                text.replace("\"", "\"\"");
                out+="\"" + text + "\"" + ",";
            }
            else
                out+="";
        }
        out += "\n";
    }
    return out;
}

void UserGeneratorWidget::generate()
{
    if(ui->tableView->model())
        ui->tableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->tableView);
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(0, 9);
    int index = 0;
    if(ui->firstNameCheckBox->isChecked())
    {
        model->setHorizontalHeaderItem(index++, new QStandardItem("First Name"));
    }
    if(ui->lastNameCheckBox->isChecked())
    {
        model->setHorizontalHeaderItem(index++, new QStandardItem("Last Name"));
    }
    if(ui->usernameCheckBox->isChecked())
    {
        model->setHorizontalHeaderItem(index++, new QStandardItem("Username"));
    }
    if(ui->emailCheckBox->isChecked())
    {
        model->setHorizontalHeaderItem(index++, new QStandardItem("Email"));
    }
    if(ui->phoneNumberCheckBox->isChecked())
    {
        model->setHorizontalHeaderItem(index++, new QStandardItem("Phone Number"));
    }
    for(int i = 0; i < ui->quantitySpinBox->value(); i++)
    {
        QList<QStandardItem*> row;
        if(ui->firstNameCheckBox->isChecked())
            row.append(new QStandardItem(getRandomQString(firstNames)));
        if(ui->lastNameCheckBox->isChecked())
            row.append(new QStandardItem(getRandomQString(lastNames)));
        if(ui->usernameCheckBox->isChecked())
        {
            QString number;
            for(int i=0; i<5; i++)
            {
                number += QString::number(random(generator));
            }
            row.append(new QStandardItem(getRandomQString(firstNames) + number));
        }
        if(ui->emailCheckBox->isChecked())
        {
            QString email;
            if(ui->firstNameCheckBox->isChecked())
            {
                email += row[0]->text();
                if(ui->lastNameCheckBox->isChecked())
                    email += row[1]->text();
                else
                    email += getRandomQString(lastNames);
            }
            else if(ui->lastNameCheckBox->isChecked())
            {
                email += getRandomQString(firstNames);
                email += row[0]->text();
            }
            else
            {
                email += getRandomQString(firstNames) + getRandomQString(lastNames);
            }
            for(int i=0; i < 5; i++)
                email += QString::number(random(generator));
            email += "@example.com";
            row.append(new QStandardItem(email.toLower()));
        }
        if(ui->phoneNumberCheckBox->isChecked())
        {
            QString number;
            for(int i=0; i<ui->phoneLength->value(); i++)
            {
                number += QString::number(random(generator));
            }
            row.append(new QStandardItem(number));
        }
        model->insertRow(i, row);
    }
    ui->tableView->setModel(model);
}
