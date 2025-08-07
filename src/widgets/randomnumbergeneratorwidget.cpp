#include "randomnumbergeneratorwidget.h"
#include "src/widgets/ui_randomnumbergeneratorwidget.h"
#include <random>
#include <limits>

RandomNumberGeneratorWidget::RandomNumberGeneratorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RandomNumberGeneratorWidget)
{
    ui->setupUi(this);
    ui->maxSpinBox->setMaximum(std::numeric_limits<int>::max());
    ui->minSpinBox->setMaximum(std::numeric_limits<int>::max());
    ui->maxSpinBox->setMinimum(std::numeric_limits<int>::min());
    ui->minSpinBox->setMinimum(std::numeric_limits<int>::min());
    ui->maxSpinBox->setValue(100);
    ui->minSpinBox->setValue(0);
    connect(ui->regenerateButton, &QPushButton::clicked, this, &RandomNumberGeneratorWidget::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->lineEdit->selectAll(); ui->lineEdit->copy();});
    connect(ui->maxSpinBox, &QSpinBox::valueChanged, this, &RandomNumberGeneratorWidget::generate);
    connect(ui->minSpinBox, &QSpinBox::valueChanged, this, &RandomNumberGeneratorWidget::generate);
    generate();
}

RandomNumberGeneratorWidget::~RandomNumberGeneratorWidget()
{
    delete ui;
}

void RandomNumberGeneratorWidget::generate()
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> random(ui->minSpinBox->value(), ui->maxSpinBox->value());
    ui->lineEdit->setText(QString::number(random(generator)));
}
