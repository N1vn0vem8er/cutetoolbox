#include "usergeneratorwidget.h"
#include "config.h"
#include "src/widgets/ui_usergeneratorwidget.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <qdialog.h>
#include <qfuture.h>
#include <qtconcurrentrun.h>
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
    ui->birthDateCheckBox->setChecked(settings.value("userGenerator.birthDate", true).toBool());
    ui->dateFromEdit->setDateTime(QDateTime::fromMSecsSinceEpoch(settings.value("userGenerator.birthDate.startDate", QDateTime::currentMSecsSinceEpoch()).toLongLong()));
    ui->dateToEdit->setDateTime(QDateTime::fromMSecsSinceEpoch(settings.value("userGenerator.birthDate.endDate", QDateTime::currentMSecsSinceEpoch()).toLongLong()));
    ui->dateFromEdit->setVisible(ui->birthDateCheckBox->isChecked());
    ui->dateToEdit->setVisible(ui->birthDateCheckBox->isChecked());
    ui->dateFromLabel->setVisible(ui->birthDateCheckBox->isChecked());
    ui->dateToLabel->setVisible(ui->birthDateCheckBox->isChecked());
    ui->phoneLength->setVisible(ui->phoneNumberCheckBox->isChecked());
    ui->phoneLengthLabel->setVisible(ui->phoneNumberCheckBox->isChecked());
    connect(ui->generateButton, &QPushButton::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->quantitySpinBox, &QSpinBox::valueChanged, this, &UserGeneratorWidget::generate);
    connect(ui->phoneLength, &QSpinBox::valueChanged, this, &UserGeneratorWidget::generate);
    connect(ui->firstNameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->lastNameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->emailCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->usernameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->phoneNumberCheckBox, &QCheckBox::clicked, this, [&](bool val){
        generate();
        ui->phoneLength->setVisible(val);
        ui->phoneLengthLabel->setVisible(val);
    });
    connect(ui->birthDateCheckBox, &QCheckBox::clicked, this, [&](bool val){
        generate();
        ui->dateFromEdit->setVisible(val);
        ui->dateToEdit->setVisible(val);
        ui->dateFromLabel->setVisible(val);
        ui->dateToLabel->setVisible(val);
    });
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{
        QGuiApplication::clipboard()->setText(toCsv());
    });
    connect(ui->saveAsButton, &QPushButton::clicked, this, &UserGeneratorWidget::saveAs);
    connect(&watcher, &QFutureWatcher<QList<QStringList>>::started, this, [&]{setUiElementsEnabled(false);});
    connect(&watcher, &QFutureWatcher<QList<QStringList>>::finished, this, [&]{
        generateTableModel(watcher.result());
    });
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
    settings.setValue("userGenerator.birthDate", ui->birthDateCheckBox->isChecked());
    settings.setValue("userGenerator.birthDate.startDate", ui->dateFromEdit->dateTime().toMSecsSinceEpoch());
    settings.setValue("userGenerator.birthDate.endDate", ui->dateToEdit->dateTime().toMSecsSinceEpoch());
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

void UserGeneratorWidget::generateTableModel(const QList<QStringList> &results)
{
    if(ui->tableView->model())
        ui->tableView->model()->deleteLater();
    int index = 0;
    QStandardItemModel* model = new QStandardItemModel(ui->tableView);
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
    if(ui->birthDateCheckBox->isChecked())
    {
        model->setHorizontalHeaderItem(index++, new QStandardItem("Birth Date"));
    }
    for(const auto& row : results)
    {
        QList<QStandardItem*> items;
        for(const auto& item : row)
        {
            items.append(new QStandardItem(item));
        }
        model->appendRow(items);
    }
    ui->tableView->setModel(model);
    setUiElementsEnabled(true);
}

void UserGeneratorWidget::generate()
{
    int quantity = ui->quantitySpinBox->value();
    bool firstName = ui->firstNameCheckBox->isChecked();
    bool lastName = ui->lastNameCheckBox->isChecked();
    bool username = ui->usernameCheckBox->isChecked();
    bool email = ui->emailCheckBox->isChecked();
    bool phone = ui->phoneNumberCheckBox->isChecked();
    bool birthDate = ui->birthDateCheckBox->isChecked();
    qint64 startDate = ui->dateFromEdit->dateTime().toMSecsSinceEpoch();
    qint64 endDate = ui->dateToEdit->dateTime().toMSecsSinceEpoch();
    auto func = [this, quantity, firstName, lastName, username, email, phone, birthDate, startDate, endDate]{
        QList<QStringList> ret;
        std::random_device rg;
        std::mt19937_64 generator(rg());
        std::uniform_int_distribution<int> random(0, 9);
        for(int i=0;i<quantity;i++)
        {
            QStringList row;
            if(firstName)
                row.append(getRandomQString(firstNames));
            if(lastName)
                row.append(getRandomQString(lastNames));
            if(username)
            {
                QString number;
                for(int i=0; i<5; i++)
                {
                    number += QString::number(random(generator));
                }
                row.append(getRandomQString(firstNames) + number);
            }
            if(email)
            {
                QString email;
                if(firstName)
                {
                    email += row[0];
                    if(lastName)
                        email += row[1];
                    else
                        email += getRandomQString(lastNames);
                }
                else if(firstName)
                {
                    email += getRandomQString(firstNames);
                    email += row[0];
                }
                else
                {
                    email += getRandomQString(firstNames) + getRandomQString(lastNames);
                }
                for(int i=0; i < 5; i++)
                    email += QString::number(random(generator));
                email += "@example.com";
                row.append(email.toLower());
            }
            if(phone)
            {
                QString number;
                for(int i=0; i<ui->phoneLength->value(); i++)
                {
                    number += QString::number(random(generator));
                }
                row.append(number);
            }
            if(birthDate)
            {
                std::uniform_int_distribution<qint64> random(startDate, endDate);
                row.append(QDateTime::fromMSecsSinceEpoch(random(generator)).toString("yyyy-MM-dd"));
            }
            ret.append(row);
        }
        return ret;
    };
    if(quantity > 1000)
    {
        QFuture<QList<QStringList>> future = QtConcurrent::run(func);
        watcher.setFuture(future);
    }
    else
        generateTableModel(func());
}

void UserGeneratorWidget::setUiElementsEnabled(bool val)
{
    ui->usernameCheckBox->setEnabled(val);
    ui->firstNameCheckBox->setEnabled(val);
    ui->lastNameCheckBox->setEnabled(val);
    ui->generateButton->setEnabled(val);
    ui->emailCheckBox->setEnabled(val);
    ui->quantitySpinBox->setEnabled(val);
    ui->phoneNumberCheckBox->setEnabled(val);
    ui->phoneLength->setEnabled(val);
    ui->generateButton->setEnabled(val);
    ui->copyButton->setEnabled(val);
    ui->saveAsButton->setEnabled(val);
    ui->dateFromEdit->setEnabled(val);
    ui->dateToEdit->setEnabled(val);
    ui->dateFromLabel->setEnabled(val);
    ui->dateToLabel->setEnabled(val);
    ui->birthDateCheckBox->setEnabled(val);
}
