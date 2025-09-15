#ifndef JAVAFORMATTERWIDGET_H
#define JAVAFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class JavaFormatterWidget;
}

class JavaFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit JavaFormatterWidget(QWidget *parent = nullptr);
    ~JavaFormatterWidget();

private:
    Ui::JavaFormatterWidget *ui;
};

#endif // JAVAFORMATTERWIDGET_H
