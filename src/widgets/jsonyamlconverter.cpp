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
    connect(ui->input, &QPlainTextEdit::textChanged, this, &JsonYamlConverter::jsonToYaml);
}

JsonYamlConverter::~JsonYamlConverter()
{
    delete ui;
}

YAML::Node JsonYamlConverter::convert(const QJsonValue &value)
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
        node.push_back(convert(value));
    }
    return node;
}

YAML::Node JsonYamlConverter::convertQJsonObjectToYaml(const QJsonObject &object)
{
    YAML::Node node;
    for(const QString& key : object.keys())
    {
        node[key.toStdString()] = convert(object.value(key));
    }
    return node;
}

void JsonYamlConverter::jsonToYaml()
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(ui->input->toPlainText().toLatin1(), &parseError);

    YAML::Node node = convert(doc.object());
    YAML::Emitter emitter;
    emitter << node;
    if(emitter.good())
        ui->output->setPlainText(QString::fromUtf8(emitter.c_str()));
}

void JsonYamlConverter::yamlToJson()
{

}
