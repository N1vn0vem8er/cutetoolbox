#include "randomtextgeneratorwidget.h"
#include "config.h"
#include "src/widgets/ui_randomtextgeneratorwidget.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <qsettings.h>
#include <qtconcurrentrun.h>
#include <random>

RandomTextGeneratorWidget::RandomTextGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RandomTextGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("Random Text Generator"));
    QSettings settings(Config::settingsName);
    ui->spinBox->setValue(settings.value("randomTextGenerator.count", 50).toInt());
    ui->upperCheckBox->setChecked(settings.value("randomTextGenerator.upper", true).toBool());
    ui->lowerCheckBox->setChecked(settings.value("randomTextGenerator.lower", true).toBool());
    ui->numbersCheckBox->setChecked(settings.value("randomTextGenerator.numbers", true).toBool());
    ui->specialCharactersCheckBox->setChecked(settings.value("randomTextGenerator.specialCharacters", false).toBool());
    connect(&watcher, &QFutureWatcher<QString>::started, this, [&]{setUiEnabled(false);});
    connect(&watcher, &QFutureWatcher<QString>::finished, this, [&]{ui->codeEditor->setPlainText(watcher.result()); setUiEnabled(true);});
    connect(ui->regenerateButton, &QPushButton::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->spinBox, &QSpinBox::editingFinished, this, &RandomTextGeneratorWidget::generate);
    connect(ui->upperCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->lowerCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->numbersCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->specialCharactersCheckBox, &QCheckBox::clicked, this, &RandomTextGeneratorWidget::generate);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->codeEditor->selectAll(); ui->codeEditor->copy();});
    generate();
}

RandomTextGeneratorWidget::~RandomTextGeneratorWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("randomTextGenerator.count", ui->spinBox->value());
    settings.setValue("randomTextGenerator.upper", ui->upperCheckBox->isChecked());
    settings.setValue("randomTextGenerator.lower", ui->lowerCheckBox->isChecked());
    settings.setValue("randomTextGenerator.numbers", ui->numbersCheckBox->isChecked());
    settings.setValue("randomTextGenerator.specialCharacters", ui->specialCharactersCheckBox->isChecked());
    delete ui;
}

bool RandomTextGeneratorWidget::canSaveFiles() const
{
    return true;
}

bool RandomTextGeneratorWidget::canBasicEdit() const
{
    return true;
}

bool RandomTextGeneratorWidget::canChangeFont() const
{
    return true;
}

void RandomTextGeneratorWidget::save()
{
    if(!openedFile.isEmpty())
    {
        QFile file(openedFile);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            emit saved(tr("Saved: %1").arg(openedFile));
        }
    }
    else
        saveAs();
}

void RandomTextGeneratorWidget::saveAs()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), !openedFile.isEmpty() ? QFileInfo(openedFile).dir().absolutePath() : QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(ui->codeEditor->toPlainText().toUtf8());
            file.close();
            openedFile = path;
            emit saved(tr("Saved: %1").arg(openedFile));
            emit opened(openedFile);
        }
    }
}

void RandomTextGeneratorWidget::close()
{
    openedFile.clear();
    emit opened(openedFile);
}

void RandomTextGeneratorWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void RandomTextGeneratorWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void RandomTextGeneratorWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void RandomTextGeneratorWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void RandomTextGeneratorWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
}

QString RandomTextGeneratorWidget::getOpenedFileName() const
{
    return openedFile;
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
    ui->codeEditor->clear();
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
