#ifndef JSONYAMLCONVERTER_H
#define JSONYAMLCONVERTER_H

#include <QWidget>

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
};

#endif // JSONYAMLCONVERTER_H
