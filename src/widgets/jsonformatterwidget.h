#ifndef JSONFORMATTERWIDGET_H
#define JSONFORMATTERWIDGET_H

#include <QWidget>

namespace Ui {
class JsonFormatterWidget;
}

class JsonFormatterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JsonFormatterWidget(QWidget *parent = nullptr);
    ~JsonFormatterWidget();

private:
    Ui::JsonFormatterWidget *ui;
};

#endif // JSONFORMATTERWIDGET_H
