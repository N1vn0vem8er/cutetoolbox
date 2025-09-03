#ifndef JSONYAMLCONVERTER_H
#define JSONYAMLCONVERTER_H

#include "customwidget.h"
#include <QWidget>
#include <yaml-cpp/yaml.h>

namespace Ui {
class JsonYamlConverter;
}

class JsonYamlConverter : public CustomWidget
{
    Q_OBJECT

public:
    explicit JsonYamlConverter(QWidget *parent = nullptr);
    ~JsonYamlConverter();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;

private:
    enum TextEdits{
        json,
        yaml,
        none
    };
    Ui::JsonYamlConverter *ui;
    bool converting {false};
    QString openedJsonFile;
    QString openedYamlFile;

private:
    YAML::Node convertQJsonValueToYaml(const QJsonValue& value);
    YAML::Node convertQJsonArrayToYaml(const QJsonArray& array);
    YAML::Node convertQJsonObjectToYaml(const QJsonObject& object);
    QJsonValue yamlNodeToJsonValue(const YAML::Node& node);
    QJsonArray yamlSequenceToJsonArray(const YAML::Node& node);
    QJsonObject yamlMapToJsonObject(const YAML::Node& node);
    TextEdits getSelectedOption();

private slots:
    void jsonToYaml();
    void yamlToJson();
};

#endif // JSONYAMLCONVERTER_H
