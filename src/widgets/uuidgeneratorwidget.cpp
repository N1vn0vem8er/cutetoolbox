#include "uuidgeneratorwidget.h"
#include "src/widgets/ui_uuidgeneratorwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QUuid>
#include <qinputdialog.h>

UUIDGeneratorWidget::UUIDGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::UUIDGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("UUID Generator"));
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

bool UUIDGeneratorWidget::canSaveFiles() const
{
    return true;
}

bool UUIDGeneratorWidget::canBasicEdit() const
{
    return true;
}

bool UUIDGeneratorWidget::canChangeFont() const
{
    return true;
}

void UUIDGeneratorWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->plainTextEdit->toPlainText().toUtf8());
            file.close();
        }
    }
    else
        saveAs();
}

void UUIDGeneratorWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->plainTextEdit->toPlainText().toUtf8());
            file.close();
            openedFile = path;
        }
    }
}

void UUIDGeneratorWidget::increaseFontSize()
{
    ui->plainTextEdit->increaseFontSize();
}

void UUIDGeneratorWidget::decreaseFontSize()
{
    ui->plainTextEdit->decreaseFontSize();
}

void UUIDGeneratorWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->plainTextEdit->setFontSize(size);
}

void UUIDGeneratorWidget::resetFontSize()
{
    ui->plainTextEdit->setFontSize(10);
}

void UUIDGeneratorWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->plainTextEdit->setFont(font);
    }
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
