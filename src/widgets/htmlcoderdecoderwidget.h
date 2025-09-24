#ifndef HTMLCODERDECODERWIDGET_H
#define HTMLCODERDECODERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class HTMLCoderDecoderWidget;
}

class HTMLCoderDecoderWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit HTMLCoderDecoderWidget(QWidget *parent = nullptr);
    ~HTMLCoderDecoderWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void close() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    QString getOpenedFileName() const override;

private:
    enum TextEdits{
        html,
        encoded,
        none
    };
    Ui::HTMLCoderDecoderWidget *ui;
    bool coding {false};
    QString openedHtmlFile;
    QString openedEncodedFile;

private:
    TextEdits getSelectedOption();

private slots:
    void encode();
    void decode();
};

#endif // HTMLCODERDECODERWIDGET_H
