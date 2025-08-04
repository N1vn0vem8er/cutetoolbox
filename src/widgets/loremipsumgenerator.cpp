#include "loremipsumgenerator.h"
#include "src/widgets/ui_loremipsumgenerator.h"
#include <random>

LoremIpsumGenerator::LoremIpsumGenerator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoremIpsumGenerator)
{
    ui->setupUi(this);
    ui->spinBox->setValue(5);
    connect(ui->spinBox, &QSpinBox::valueChanged, this, &LoremIpsumGenerator::generate);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &LoremIpsumGenerator::generate);
    connect(ui->beginWithLICheckBox, &QCheckBox::clicked, this, &LoremIpsumGenerator::generate);
    connect(ui->regenerateButton, &QPushButton::clicked, this, &LoremIpsumGenerator::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->plainTextEdit->selectAll(); ui->plainTextEdit->copy();});
    generate();
}

LoremIpsumGenerator::~LoremIpsumGenerator()
{
    delete ui;
}

QString LoremIpsumGenerator::generateSentence(int minWords, int maxWords, bool makeFirstCharUpper) const
{
    std::random_device rg;
    std::mt19937_64 generator(rg());
    std::uniform_int_distribution<int> wordCountDistribution(minWords, maxWords);
    std::uniform_int_distribution<int> wordIndexDistribution(0, words.size() - 1);
    std::uniform_int_distribution<int> punctuationDistribution(0, 9);

    const int quantity = wordCountDistribution(generator);
    QString out;
    for(int i = 0 ; i < quantity; i++)
    {
        QString word = words.at(wordIndexDistribution(generator));
        if(makeFirstCharUpper && i == 0)
        {
            word[0] = word.at(0).toUpper();
        }
        out.append(word);
        if(i != quantity-1)
            out.append(" ");
    }
    int end = punctuationDistribution(generator);
    if(end < 7)
        out.append(".");
    else if(end < 9)
        out.append("?");
    else
        out.append("!");
    return out;
}

void LoremIpsumGenerator::generate()
{
    switch(ui->comboBox->currentIndex())
    {
    case 0:
        ui->plainTextEdit->setPlainText((ui->beginWithLICheckBox->isChecked() ? "Lorem ipsum dolor sit amet, " : "") + generateSentence(ui->spinBox->value(), ui->spinBox->value(), !ui->beginWithLICheckBox->isChecked()));
        break;
    case 1:
    {
        ui->plainTextEdit->clear();
        int i = 0;
        if(ui->beginWithLICheckBox->isChecked())
        {
            ui->plainTextEdit->appendPlainText("Lorem ipsum dolor sit amet, " + generateSentence(5, 30, false));
            i = 1;
        }
        for(; i < ui->spinBox->value(); i++)
        {
            ui->plainTextEdit->appendPlainText(generateSentence(5, 20));
        }
    }
        break;
    case 2:
    {
        std::random_device rg;
        std::mt19937_64 generator(rg());
        std::uniform_int_distribution<int> sentenceCount(5, 30);
        ui->plainTextEdit->clear();
        for(int i = 0; i < ui->spinBox->value(); i++)
        {
            int sentence = 0;
            if(ui->beginWithLICheckBox->isChecked())
            {
                ui->plainTextEdit->appendPlainText("Lorem ipsum dolor sit amet, " + generateSentence(5, 30, false));
                sentence = 1;
            }
            int sentences = sentenceCount(generator);
            for(; sentence < sentences; sentence++)
            {
                ui->plainTextEdit->appendPlainText(generateSentence(5, 20));
            }
            ui->plainTextEdit->appendPlainText("\n");
        }
    }
        break;
    }
}
