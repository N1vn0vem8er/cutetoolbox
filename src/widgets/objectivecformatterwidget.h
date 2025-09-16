#ifndef OBJECTIVECFORMATTERWIDGET_H
#define OBJECTIVECFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ObjectiveCFormatterWidget;
}

class ObjectiveCFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ObjectiveCFormatterWidget(QWidget *parent = nullptr);
    ~ObjectiveCFormatterWidget();

private:
    Ui::ObjectiveCFormatterWidget *ui;
};

#endif // OBJECTIVECFORMATTERWIDGET_H
