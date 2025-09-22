#include "jsonyamlconverter.h"
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

JsonYamlConverter::JsonYamlConverter(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::JsonYamlConverter)
{
    ui->setupUi(this);
    setName(tr("JSON - YAML Converter"));
    ui->yamlIndentations->setValue(4);
    connect(ui->json, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::jsonToYaml);
    connect(ui->yaml, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::yamlToJson);
    connect(ui->yamlIndentations, &QSpinBox::valueChanged, this, &JsonYamlConverter::jsonToYaml);
}

JsonYamlConverter::~JsonYamlConverter()
{
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
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
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
            }
        }
    }
}

void JsonYamlConverter::open()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly))
            {
                if(option == TextEdits::json)
                {
                    ui->json->setPlainText(file.readAll());
                    openedJsonFile = path;
                    emit opened(openedJsonFile + " " + openedYamlFile);
                }
                else if(option == TextEdits::yaml)
                {
                    ui->yaml->setPlainText(file.readAll());
                    openedYamlFile = path;
                    emit opened(openedJsonFile + " " + openedYamlFile);
                }
            }
        }
    }
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
