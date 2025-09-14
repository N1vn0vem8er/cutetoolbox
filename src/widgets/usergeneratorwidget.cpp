#include "usergeneratorwidget.h"
#include "src/widgets/ui_usergeneratorwidget.h"
#include <QStandardItemModel>
#include <qdialog.h>
#include <random>

UserGeneratorWidget::UserGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::UserGeneratorWidget)
{
    ui->setupUi(this);
    connect(ui->generateButton, &QPushButton::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->quantitySpinBox, &QSpinBox::valueChanged, this, &UserGeneratorWidget::generate);
    connect(ui->phoneLength, &QSpinBox::valueChanged, this, &UserGeneratorWidget::generate);
    connect(ui->firstNameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->lastNameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->emailCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->usernameCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
    connect(ui->phoneNumberCheckBox, &QCheckBox::clicked, this, &UserGeneratorWidget::generate);
}

UserGeneratorWidget::~UserGeneratorWidget()
{
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

}

void UserGeneratorWidget::saveAs()
{

}

QString UserGeneratorWidget::getRandomQString(const QStringList &list) const
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(0, list.length() - 1);
    return list[random(generator)];
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
            row.append(new QStandardItem(getRandomQString(firstNames) + getRandomQString(lastNames) + number));
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
