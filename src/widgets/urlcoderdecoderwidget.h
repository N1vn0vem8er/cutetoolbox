#ifndef URLCODERDECODERWIDGET_H
#define URLCODERDECODERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class UrlCoderDecoderWidget;
}

class UrlCoderDecoderWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit UrlCoderDecoderWidget(QWidget *parent = nullptr);
    ~UrlCoderDecoderWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void deleteText() override;
    void deleteAllText() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;

private:
    enum TextEdits{
        decoded,
        encoded,
        none
    };
    Ui::UrlCoderDecoderWidget *ui;
    bool coding {false};
    TextEdits getSelectedOption();

private slots:
    void encode();
    void decode();
};

#endif // URLCODERDECODERWIDGET_H
