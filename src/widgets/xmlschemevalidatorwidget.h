#ifndef XMLSCHEMEVALIDATORWIDGET_H
#define XMLSCHEMEVALIDATORWIDGET_H

#include <QWidget>
#include <libxml/xmlschemas.h>
#include <qlabel.h>

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
    QStringList errors;
    QLabel* infoLabel {nullptr};

private:
    static void handleErrorMsg(void* userData, xmlErrorPtr error);

private slots:
    void validate();
};

#endif // XMLSCHEMEVALIDATORWIDGET_H
