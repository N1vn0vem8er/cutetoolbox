#include "jsonyamlconverter.h"
#include "src/widgets/ui_jsonyamlconverter.h"
#include <QJsonValue>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QJsonArray>

JsonYamlConverter::JsonYamlConverter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::JsonYamlConverter)
{
    ui->setupUi(this);
    connect(ui->json, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::jsonToYaml);
    connect(ui->yaml, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::yamlToJson);
}

JsonYamlConverter::~JsonYamlConverter()
{
    delete ui;
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

void JsonYamlConverter::jsonToYaml()
{
    if(!converting)
    {
        converting = true;
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(ui->json->toPlainText().toLatin1(), &parseError);
        YAML::Node node = convertQJsonValueToYaml(doc.object());
        YAML::Emitter emitter;
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
