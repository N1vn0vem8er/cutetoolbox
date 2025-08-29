#ifndef JSONYAMLCONVERTER_H
#define JSONYAMLCONVERTER_H

#include <QWidget>
#include <yaml-cpp/yaml.h>

namespace Ui {
class JsonYamlConverter;
}

class JsonYamlConverter : public QWidget
{
    Q_OBJECT

public:
    explicit JsonYamlConverter(QWidget *parent = nullptr);
    ~JsonYamlConverter();

private:
    Ui::JsonYamlConverter *ui;
    bool converting {false};

private:
    YAML::Node convertQJsonValueToYaml(const QJsonValue& value);
    YAML::Node convertQJsonArrayToYaml(const QJsonArray& array);
    YAML::Node convertQJsonObjectToYaml(const QJsonObject& object);
    QJsonValue yamlNodeToJsonValue(const YAML::Node& node);
    QJsonArray yamlSequenceToJsonArray(const YAML::Node& node);
    QJsonObject yamlMapToJsonObject(const YAML::Node& node);

private slots:
    void jsonToYaml();
    void yamlToJson();
};

#endif // JSONYAMLCONVERTER_H
