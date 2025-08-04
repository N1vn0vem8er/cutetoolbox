#include "uuidgeneratorwidget.h"
#include "src/widgets/ui_uuidgeneratorwidget.h"

#include <QUuid>

UUIDGeneratorWidget::UUIDGeneratorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UUIDGeneratorWidget)
{
    ui->setupUi(this);
    ui->spinBox->setValue(5);
    ui->dataLabel->setVisible(false);
    ui->dataLineEdit->setVisible(false);
    ui->namespaceLineEdit->setVisible(false);
    ui->namespaceLabel->setVisible(false);
    connect(ui->spinBox, &QSpinBox::valueChanged, this, &UUIDGeneratorWidget::generate);
    connect(ui->regenerateButton, &QPushButton::clicked, this, &UUIDGeneratorWidget::generate);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &UUIDGeneratorWidget::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->plainTextEdit->selectAll(); ui->plainTextEdit->copy();});
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, [&](int index){
        if(index == 0)
        {
            ui->dataLabel->setVisible(false);
            ui->dataLineEdit->setVisible(false);
            ui->namespaceLineEdit->setVisible(false);
            ui->namespaceLabel->setVisible(false);
        }
        else
        {
            ui->dataLabel->setVisible(true);
            ui->dataLineEdit->setVisible(true);
            ui->namespaceLineEdit->setVisible(true);
            ui->namespaceLabel->setVisible(true);
        }
    });
    generate();
}

UUIDGeneratorWidget::~UUIDGeneratorWidget()
{
    delete ui;
}

void UUIDGeneratorWidget::generate()
{
    ui->plainTextEdit->clear();
    for(int i=0; i<ui->spinBox->value(); i++)
    {
        switch(ui->comboBox->currentIndex())
        {
        case 0:
            ui->plainTextEdit->appendPlainText(QUuid::createUuid().toString(QUuid::WithoutBraces));
            break;
        case 1:
            ui->plainTextEdit->appendPlainText(QUuid::createUuidV3(QUuid(ui->namespaceLineEdit->text()), ui->dataLineEdit->text()).toString(QUuid::WithoutBraces));
            break;
        case 2:
            ui->plainTextEdit->appendPlainText(QUuid::createUuidV5(QUuid(ui->namespaceLineEdit->text()), ui->dataLineEdit->text()).toString(QUuid::WithoutBraces));
            break;
        }
    }
}
