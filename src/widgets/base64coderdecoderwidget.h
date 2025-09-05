#ifndef BASE64CODERDECODERWIDGET_H
#define BASE64CODERDECODERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class Base64CoderDecoderWidget;
}

class Base64CoderDecoderWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit Base64CoderDecoderWidget(QWidget *parent = nullptr);
    ~Base64CoderDecoderWidget();
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
        text,
        base64,
        none
    };
    Ui::Base64CoderDecoderWidget *ui;
    bool coding {false};
    QString openedTextFile;
    QString openedBase64File;

private:
    TextEdits getSelectedOption();

private slots:
    void encode();
    void decode();
};

#endif // BASE64CODERDECODERWIDGET_H
