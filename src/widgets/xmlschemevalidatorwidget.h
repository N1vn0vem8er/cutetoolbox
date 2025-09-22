#ifndef XMLSCHEMEVALIDATORWIDGET_H
#define XMLSCHEMEVALIDATORWIDGET_H

#include "customwidget.h"
#include <QWidget>
#include <libxml/xmlschemas.h>
#include <qlabel.h>

namespace Ui {
class XMLSchemeValidatorWidget;
}

class XMLSchemeValidatorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit XMLSchemeValidatorWidget(QWidget *parent = nullptr);
    ~XMLSchemeValidatorWidget();
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
    QString getOpenedFileName() const override;

private:
    enum TextEdits{
        xml,
        xsd,
        none
    };
    Ui::XMLSchemeValidatorWidget *ui;
    QStringList errors;
    QLabel* infoLabel {nullptr};
    QString openedXmlFile;
    QString openedXsdFile;

private:
    static void handleErrorMsg(void* userData, xmlErrorPtr error);
    TextEdits getSelectedOption();

private slots:
    void validate();
};

#endif // XMLSCHEMEVALIDATORWIDGET_H
