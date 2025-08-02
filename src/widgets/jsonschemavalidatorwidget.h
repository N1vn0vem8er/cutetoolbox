#ifndef JSONSCHEMAVALIDATORWIDGET_H
#define JSONSCHEMAVALIDATORWIDGET_H

#include <QWidget>

namespace Ui {
class JSONSchemaValidatorWidget;
}

class JSONSchemaValidatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JSONSchemaValidatorWidget(QWidget *parent = nullptr);
    ~JSONSchemaValidatorWidget();

private:
    Ui::JSONSchemaValidatorWidget *ui;
};

#endif // JSONSCHEMAVALIDATORWIDGET_H
