#ifndef XMLSCHEMEVALIDATORWIDGET_H
#define XMLSCHEMEVALIDATORWIDGET_H

#include <QWidget>

namespace Ui {
class XMLSchemeValidatorWidget;
}

class XMLSchemeValidatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XMLSchemeValidatorWidget(QWidget *parent = nullptr);
    ~XMLSchemeValidatorWidget();

private:
    Ui::XMLSchemeValidatorWidget *ui;
};

#endif // XMLSCHEMEVALIDATORWIDGET_H
