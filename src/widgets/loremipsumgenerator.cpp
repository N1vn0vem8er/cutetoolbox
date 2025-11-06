#include "loremipsumgenerator.h"
#include "config.h"
#include "src/widgets/ui_loremipsumgenerator.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qsettings.h>
#include <random>

LoremIpsumGenerator::LoremIpsumGenerator(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::LoremIpsumGenerator)
{
    ui->setupUi(this);
    setName(tr("Lorem Ipsum Generator"));
    QSettings settings(Config::settingsName);
    ui->spinBox->setValue(settings.value("loremIpsumGenerator.count", 5).toInt());
    ui->comboBox->setCurrentIndex(settings.value("loremIpsumGenerator.type", 0).toInt());
    ui->beginWithLICheckBox->setChecked(settings.value("loremIpsumGenerator.beginWithLI", false).toBool());
    connect(ui->spinBox, &QSpinBox::valueChanged, this, &LoremIpsumGenerator::generate);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &LoremIpsumGenerator::generate);
    connect(ui->beginWithLICheckBox, &QCheckBox::clicked, this, &LoremIpsumGenerator::generate);
    connect(ui->regenerateButton, &QPushButton::clicked, this, &LoremIpsumGenerator::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->output->selectAll(); ui->output->copy();});
    generate();
}

LoremIpsumGenerator::~LoremIpsumGenerator()
{
    QSettings settings(Config::settingsName);
    settings.setValue("loremIpsumGenerator.count", ui->spinBox->value());
    settings.setValue("loremIpsumGenerator.type", ui->comboBox->currentIndex());
    settings.setValue("loremIpsumGenerator.beginWithLI", ui->beginWithLICheckBox->isChecked());
    delete ui;
}

bool LoremIpsumGenerator::canSaveFiles() const
{
    return true;
}

bool LoremIpsumGenerator::canChangeFont() const
{
    return true;
}

bool LoremIpsumGenerator::canBasicEdit() const
{
    return true;
}

void LoremIpsumGenerator::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->output->toPlainText().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void LoremIpsumGenerator::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->output->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void LoremIpsumGenerator::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void LoremIpsumGenerator::increaseFontSize()
{
    ui->output->increaseFontSize();
}

void LoremIpsumGenerator::decreaseFontSize()
{
    ui->output->decreaseFontSize();
}

void LoremIpsumGenerator::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->output->setFontSize(size);
}

void LoremIpsumGenerator::resetFontSize()
{
    ui->output->setFontSize(10);
}

void LoremIpsumGenerator::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->output->setFont(font);
    }
}

QString LoremIpsumGenerator::getOpenedFileName() const
{
    return openedFile;
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
        ui->output->setPlainText((ui->beginWithLICheckBox->isChecked() ? "Lorem ipsum dolor sit amet, " : "") + generateSentence(ui->spinBox->value(), ui->spinBox->value(), !ui->beginWithLICheckBox->isChecked()));
        break;
    case 1:
    {
        ui->output->clear();
        int i = 0;
        if(ui->beginWithLICheckBox->isChecked())
        {
            ui->output->appendPlainText("Lorem ipsum dolor sit amet, " + generateSentence(5, 30, false));
            i = 1;
        }
        for(; i < ui->spinBox->value(); i++)
        {
            ui->output->appendPlainText(generateSentence(5, 20));
        }
    }
        break;
    case 2:
    {
        std::random_device rg;
        std::mt19937_64 generator(rg());
        std::uniform_int_distribution<int> sentenceCount(5, 30);
        ui->output->clear();
        for(int i = 0; i < ui->spinBox->value(); i++)
        {
            int sentence = 0;
            if(ui->beginWithLICheckBox->isChecked())
            {
                ui->output->appendPlainText("Lorem ipsum dolor sit amet, " + generateSentence(5, 30, false));
                sentence = 1;
            }
            int sentences = sentenceCount(generator);
            for(; sentence < sentences; sentence++)
            {
                ui->output->appendPlainText(generateSentence(5, 20));
            }
            ui->output->appendPlainText("\n");
        }
    }
        break;
    }
}
