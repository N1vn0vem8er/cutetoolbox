#ifndef PROTOFORMATTERWIDGET_H
#define PROTOFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ProtoFormatterWidget;
}

class ProtoFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ProtoFormatterWidget(QWidget *parent = nullptr);
    ~ProtoFormatterWidget();

private:
    Ui::ProtoFormatterWidget *ui;
};

#endif // PROTOFORMATTERWIDGET_H
