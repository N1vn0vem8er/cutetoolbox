#include "randomnumbergeneratorwidget.h"
#include "config.h"
#include "src/widgets/ui_randomnumbergeneratorwidget.h"
#include <qsettings.h>
#include <random>
#include <limits>

RandomNumberGeneratorWidget::RandomNumberGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RandomNumberGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("Random Number Generator"));
    ui->maxSpinBox->setMaximum(std::numeric_limits<int>::max());
    ui->minSpinBox->setMaximum(std::numeric_limits<int>::max());
    ui->maxSpinBox->setMinimum(std::numeric_limits<int>::min());
    ui->minSpinBox->setMinimum(std::numeric_limits<int>::min());
    QSettings settings(Config::settingsName);
    ui->maxSpinBox->setValue(settings.value("randomNumberGenerator.maxValue", 100).toInt());
    ui->minSpinBox->setValue(settings.value("randomNumberGenerator.minValue", 0).toInt());
    connect(ui->regenerateButton, &QPushButton::clicked, this, &RandomNumberGeneratorWidget::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->lineEdit->selectAll(); ui->lineEdit->copy();});
    connect(ui->maxSpinBox, &QSpinBox::valueChanged, this, &RandomNumberGeneratorWidget::generate);
    connect(ui->minSpinBox, &QSpinBox::valueChanged, this, &RandomNumberGeneratorWidget::generate);
    generate();
}

RandomNumberGeneratorWidget::~RandomNumberGeneratorWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("randomNumberGenerator.maxValue", ui->maxSpinBox->value());
    settings.setValue("randomNumberGenerator.minValue", ui->minSpinBox->value());
    delete ui;
}

bool RandomNumberGeneratorWidget::canBasicEdit() const
{
    return true;
}

void RandomNumberGeneratorWidget::generate()
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(ui->minSpinBox->value(), ui->maxSpinBox->value());
    ui->lineEdit->setText(QString::number(random(generator)));
}
