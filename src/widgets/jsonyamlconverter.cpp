#include "jsonyamlconverter.h"
#include "config.h"
#include "src/widgets/ui_jsonyamlconverter.h"
#include <QJsonValue>
#include <qdialog.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QJsonArray>
#include <qpushbutton.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QFontDialog>
#include <QSettings>

JsonYamlConverter::JsonYamlConverter(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JsonYamlConverter)
{
    ui->setupUi(this);
    setName(tr("JSON - YAML Converter"));
    QSettings settings(Config::settingsName);
    ui->yamlIndentations->setValue(settings.value("jsonYamlConverter.yamlIndentations", 4).toInt());
    connect(ui->json, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::jsonToYaml);
    connect(ui->yaml, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::yamlToJson);
    connect(ui->yamlIndentations, &QSpinBox::valueChanged, this, &JsonYamlConverter::jsonToYaml);
    connect(ui->openJsonButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.json");
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                ui->json->setPlainText(file.readAll());
                file.close();
                openedJsonFile = path;
                if(recentJsonFiles.length() >= 10)
                    recentJsonFiles.removeFirst();
                if(!recentJsonFiles.contains(openedJsonFile))
                    recentJsonFiles.append(openedJsonFile);
                emit updateRecent();
                emit opened(openedJsonFile + " " + openedYamlFile);
            }
        }
    });
    connect(ui->openYamlButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.yaml");
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                ui->yaml->setPlainText(file.readAll());
                file.close();
                openedYamlFile = path;
                if(recentYamlFiles.length() >= 10)
                    recentYamlFiles.removeFirst();
                if(!recentYamlFiles.contains(openedYamlFile))
                    recentYamlFiles.append(openedYamlFile);
                emit updateRecent();
                emit opened(openedJsonFile + " " + openedYamlFile);
            }
        }
    });
    connect(ui->copyJsonButton, &QPushButton::clicked, ui->json, &CodeEditor::copyAll);
    connect(ui->copyYamlButton, &QPushButton::clicked, ui->yaml, &CodeEditor::copyAll);
    connect(ui->pasteJsonButton, &QPushButton::clicked, ui->json, &CodeEditor::paste);
    connect(ui->pasteYamlButton, &QPushButton::clicked, ui->yaml, &CodeEditor::paste);
    connect(ui->clearJsonButton, &QPushButton::clicked, ui->json, &CodeEditor::clear);
    connect(ui->clearYamlButton, &QPushButton::clicked, ui->yaml, &CodeEditor::clear);
    int size = settings.beginReadArray("jsonYamlConverter.recentJsonFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentJsonFiles.append(path);
    }
    settings.endArray();
    size = settings.beginReadArray("jsonYamlConverter.recentYamlFiles");
    for(int i = 0; i<size; i++)
    {
        settings.setArrayIndex(i);
        const QString path = settings.value("path").toString();
        if(!path.isEmpty())
            recentYamlFiles.append(path);
    }
    settings.endArray();
}

JsonYamlConverter::~JsonYamlConverter()
{
    QSettings settings(Config::settingsName);
    settings.setValue("jsonYamlConverter.yamlIndentations", ui->yamlIndentations->value());
    settings.beginWriteArray("jsonYamlConverter.recentJsonFiles");
    for(int i = 0; i<recentJsonFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentJsonFiles.at(i));
    }
    settings.endArray();
    settings.beginWriteArray("jsonYamlConverter.recentYamlFiles");
    for(int i = 0; i<recentYamlFiles.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("path", recentYamlFiles.at(i));
    }
    settings.endArray();
    delete ui;
}

bool JsonYamlConverter::canOpenFiles() const
{
    return true;
}

bool JsonYamlConverter::canSaveFiles() const
{
    return true;
}

bool JsonYamlConverter::canBasicEdit() const
{
    return true;
}

bool JsonYamlConverter::canChangeFont() const
{
    return true;
}

void JsonYamlConverter::save()
{
    if(!openedJsonFile.isEmpty() || !openedYamlFile.isEmpty())
    {
        TextEdits option = TextEdits::none;
        if(ui->json->hasFocus() && !openedJsonFile.isEmpty())
            option = json;
        else if(ui->yaml->hasFocus() && !openedYamlFile.isEmpty())
            option = yaml;
        else if(!openedJsonFile.isEmpty() && !openedYamlFile.isEmpty())
            option = getSelectedOption();
        else if(!openedJsonFile.isEmpty())
            option = json;
        else if(!openedYamlFile.isEmpty())
            option = yaml;
        if(option != TextEdits::none)
        {
            if(option == TextEdits::json)
            {
                QFile file(openedJsonFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->json->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedJsonFile));
                }
            }
            else if(option == TextEdits::yaml)
            {
                QFile file(openedYamlFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->yaml->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedYamlFile));
                }
            }
        }
    }
    else
        saveAs();
}

void JsonYamlConverter::saveAs()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), option == TextEdits::json ? "*.json" : "*.yaml");
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                if(option == TextEdits::json)
                {
                    file.write(ui->json->toPlainText().toUtf8());
                    openedJsonFile = path;
                    emit saved(tr("Saved: %1").arg(openedJsonFile));
                }
                else if(option == TextEdits::yaml)
                {
                    file.write(ui->yaml->toPlainText().toUtf8());
                    openedYamlFile = path;
                    emit saved(tr("Saved: %1").arg(openedYamlFile));
                }
                file.close();
                emit opened(openedJsonFile + " " + openedYamlFile);
            }
        }
    }
}

void JsonYamlConverter::open()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), option == TextEdits::json ? "*.json" : "*.yaml");
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                if(option == TextEdits::json)
                {
                    ui->json->setPlainText(file.readAll());
                    openedJsonFile = path;
                    if(recentJsonFiles.length() >= 10)
                        recentJsonFiles.removeFirst();
                    if(!recentJsonFiles.contains(openedJsonFile))
                        recentJsonFiles.append(openedJsonFile);
                }
                else if(option == TextEdits::yaml)
                {
                    ui->yaml->setPlainText(file.readAll());
                    openedYamlFile = path;
                    if(recentYamlFiles.length() >= 10)
                        recentYamlFiles.removeFirst();
                    if(!recentYamlFiles.contains(openedYamlFile))
                        recentYamlFiles.append(openedYamlFile);
                }
                file.close();
                emit opened(openedJsonFile + " " + openedYamlFile);
                emit updateRecent();
            }
        }
    }
}

void JsonYamlConverter::close()
{
    if(openedJsonFile.isEmpty())
        openedYamlFile.clear();
    else if(openedYamlFile.isEmpty())
        openedJsonFile.clear();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::json)
                openedJsonFile.clear();
            else if(option == TextEdits::yaml)
                openedYamlFile.clear();
        }
    }
    emit opened(openedJsonFile + " " + openedYamlFile);
}

void JsonYamlConverter::increaseFontSize()
{
    if(ui->json->hasFocus())
        ui->json->increaseFontSize();
    else if(ui->yaml->hasFocus())
        ui->yaml->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::json)
                ui->json->increaseFontSize();
            else if(option == TextEdits::yaml)
                ui->yaml->increaseFontSize();
        }
    }
}

void JsonYamlConverter::decreaseFontSize()
{
    if(ui->json->hasFocus())
        ui->json->decreaseFontSize();
    else if(ui->yaml->hasFocus())
        ui->yaml->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::json)
                ui->json->decreaseFontSize();
            else if(option == TextEdits::yaml)
                ui->yaml->decreaseFontSize();
        }
    }
}

void JsonYamlConverter::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200);
        if(option == TextEdits::json)
            ui->json->setFontSize(size);
        else if(option == TextEdits::yaml)
            ui->yaml->setFontSize(size);
    }
}

void JsonYamlConverter::resetFontSize()
{
    if(ui->json->hasFocus())
        ui->json->setFontSize(10);
    else if(ui->yaml->hasFocus())
        ui->yaml->setFontSize(10);
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::json)
                ui->json->setFontSize(10);
            else if(option == TextEdits::yaml)
                ui->yaml->setFontSize(10);
        }
    }
}

void JsonYamlConverter::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::json)
                ui->json->setFont(font);
            else if(option == TextEdits::yaml)
                ui->yaml->setFont(font);
        }
    }
}

QString JsonYamlConverter::getOpenedFileName() const
{
    return openedJsonFile + " " + openedYamlFile;
}

QStringList JsonYamlConverter::getRecentFiles() const
{
    return recentJsonFiles + recentYamlFiles;
}

void JsonYamlConverter::openFromRecent(const QString &path)
{
    if(recentJsonFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->json->setPlainText(file.readAll());
            file.close();
            openedJsonFile = path;
            emit opened(openedJsonFile + " " + openedYamlFile);
        }
    }
    else if(recentYamlFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            ui->yaml->setPlainText(file.readAll());
            file.close();
            openedYamlFile = path;
            emit opened(openedJsonFile + " " + openedYamlFile);
        }
    }
}

void JsonYamlConverter::clearRecent()
{
    recentJsonFiles.clear();
    recentYamlFiles.clear();
    emit updateRecent();
}

YAML::Node JsonYamlConverter::convertQJsonValueToYaml(const QJsonValue &value)
{
    YAML::Node node;
    switch(value.type())
    {
    case QJsonValue::Null:
        node = YAML::Null;
        break;
    case QJsonValue::Bool:
        node = value.toBool();
        break;
    case QJsonValue::Double:
        node = value.toDouble();
        break;
    case QJsonValue::String:
        node = value.toString().toStdString();
        break;
    case QJsonValue::Array:
        node = convertQJsonArrayToYaml(value.toArray());
        break;
    case QJsonValue::Object:
        node = convertQJsonObjectToYaml(value.toObject());
        break;
    case QJsonValue::Undefined:
        break;
    }
    return node;
}

YAML::Node JsonYamlConverter::convertQJsonArrayToYaml(const QJsonArray &array)
{
    YAML::Node node(YAML::NodeType::Sequence);
    for(const QJsonValue& value : array)
    {
        node.push_back(convertQJsonValueToYaml(value));
    }
    return node;
}

YAML::Node JsonYamlConverter::convertQJsonObjectToYaml(const QJsonObject &object)
{
    YAML::Node node;
    for(const QString& key : object.keys())
    {
        node[key.toStdString()] = convertQJsonValueToYaml(object.value(key));
    }
    return node;
}

QJsonValue JsonYamlConverter::yamlNodeToJsonValue(const YAML::Node &node)
{
    if (node.IsMap())
    {
        return yamlMapToJsonObject(node);
    }
    else if (node.IsSequence())
    {
        return yamlSequenceToJsonArray(node);
    }
    else if (node.IsScalar())
    {
        bool ok = false;
        bool boolValue = node.as<bool>(ok);
        if(ok)
            return QJsonValue(boolValue);
        int intValue = node.as<int>(ok);
        if(ok)
            return QJsonValue(intValue);
        double doubleValue = node.as<double>(ok);
        if(ok)
            return QJsonValue(doubleValue);
        return QJsonValue(QString::fromStdString(node.as<std::string>()));
    }
    else if (node.IsNull())
    {
        return QJsonValue(QJsonValue::Null);
    }

    return QJsonValue();
}

QJsonArray JsonYamlConverter::yamlSequenceToJsonArray(const YAML::Node &node)
{
    QJsonArray jsonArray;
    if (!node.IsSequence())
    {
        return jsonArray;
    }
    for (const auto& item : node)
    {
        jsonArray.append(yamlNodeToJsonValue(item));
    }
    return jsonArray;
}

QJsonObject JsonYamlConverter::yamlMapToJsonObject(const YAML::Node &node)
{
    QJsonObject jsonObject;
    if (!node.IsMap())
    {
        return jsonObject;
    }
    for (YAML::const_iterator it = node.begin(); it != node.end(); it++)
    {
        QString key = QString::fromStdString(it->first.as<std::string>());
        QJsonValue value = yamlNodeToJsonValue(it->second);
        jsonObject[key] = value;
    }
    return jsonObject;
}

JsonYamlConverter::TextEdits JsonYamlConverter::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton jsonButton(tr("Json"), &dialog);
    QPushButton yamlButton(tr("Yaml"), &dialog);
    connect(&jsonButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::json;});
    connect(&yamlButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::yaml;});
    connect(&jsonButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&yamlButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&jsonButton);
    layout.addWidget(&yamlButton);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
}

void JsonYamlConverter::jsonToYaml()
{
    if(!converting)
    {
        converting = true;
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(ui->json->toPlainText().toLatin1(), &parseError);
        YAML::Node node = convertQJsonValueToYaml(doc.object());
        YAML::Emitter emitter;
        emitter.SetIndent(ui->yamlIndentations->value());
        emitter << node;
        if(emitter.good())
            ui->yaml->setPlainText(QString::fromUtf8(emitter.c_str()));
        ui->errorLabel->clear();
    }
    else
    {
        converting = false;
    }
}

void JsonYamlConverter::yamlToJson()
{
    if(!converting)
    {
        converting = true;
        try
        {
            YAML::Node node = YAML::Load(ui->yaml->toPlainText().toStdString());
            QJsonDocument doc(yamlNodeToJsonValue(node).toObject());
            ui->json->setPlainText(doc.toJson());
            ui->errorLabel->clear();
        }
        catch(const YAML::ParserException& e)
        {
            ui->errorLabel->setText(QString::fromStdString(e.msg));
        }
    }
    else
    {
        converting = false;
    }
}
