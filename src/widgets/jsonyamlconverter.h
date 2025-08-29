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

private:
    YAML::Node convert(const QJsonValue& value);
    YAML::Node convertQJsonArrayToYaml(const QJsonArray& array);
    YAML::Node convertQJsonObjectToYaml(const QJsonObject& object);

private slots:
    void jsonToYaml();
    void yamlToJson();
};

#endif // JSONYAMLCONVERTER_H
