#include "randomtextgeneratorwidget.h"
#include "src/widgets/ui_randomtextgeneratorwidget.h"
#include <qtconcurrentrun.h>
#include <random>

RandomTextGeneratorWidget::RandomTextGeneratorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RandomTextGeneratorWidget)
{
    ui->setupUi(this);
    ui->spinBox->setValue(50);
    ui->upperCheckBox->setChecked(true);
    ui->lowerCheckBox->setChecked(true);
    ui->numbersCheckBox->setChecked(true);
    connect(&watcher, &QFutureWatcher<QString>::started, this, [&]{setUiEnabled(false);});
    connect(&watcher, &QFutureWatcher<QString>::finished, this, [&]{ui->plainTextEdit->setPlainText(watcher.result()); setUiEnabled(true);});
    connect(ui->regenerateButton, &QPushButton::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->spinBox, &QSpinBox::editingFinished, this, &RandomTextGeneratorWidget::generate);
    connect(ui->upperCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->lowerCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->numbersCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->specialCharactersCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->plainTextEdit->selectAll(); ui->plainTextEdit->copy();});
    generate();
}

RandomTextGeneratorWidget::~RandomTextGeneratorWidget()
{
    delete ui;
}

void RandomTextGeneratorWidget::setUiEnabled(bool val)
{
    ui->upperCheckBox->setEnabled(val);
    ui->lowerCheckBox->setEnabled(val);
    ui->spinBox->setEnabled(val);
    ui->numbersCheckBox->setEnabled(val);
    ui->specialCharactersCheckBox->setEnabled(val);
    ui->regenerateButton->setEnabled(val);
    ui->copyButton->setEnabled(val);
}

void RandomTextGeneratorWidget::generate()
{
    QList<QChar> characters;
    if(ui->lowerCheckBox->isChecked())
    {
        QList<QChar> lowercase;
        lowercase.reserve(letters.size());
        std::transform(letters.begin(), letters.end(), std::back_inserter(lowercase), [](QChar ch){return ch.toLower();});
        characters.append(lowercase);
    }
    if(ui->upperCheckBox->isChecked())
    {
        characters.append(letters);
    }
    if(ui->numbersCheckBox->isChecked())
    {
        characters.append(numbers);
    }
    if(ui->specialCharactersCheckBox->isChecked())
    {
        characters.append(specialCharacters);
    }
    ui->plainTextEdit->clear();
    if(characters.isEmpty()) return;

    const int length = ui->spinBox->value();
    QFuture<QString> future = QtConcurrent::run([length, characters]{
        QString result;
        if(characters.isEmpty()) return result;
        result.reserve(length);
        std::random_device rg;
        std::mt19937_64 generator(rg());
        std::uniform_int_distribution<int> dist(0, characters.size() - 1);
        for(int i = 0; i < length; i++)
        {
            result.append(characters.at(dist(generator)));
        }
        return result;
    });
    watcher.setFuture(future);
}

void RandomTextGeneratorWidget::generationFinished()
{

}
