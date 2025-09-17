#ifndef TEXTPROTOFORMATTERWIDGET_H
#define TEXTPROTOFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class TextProtoFormatterWidget;
}

class TextProtoFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit TextProtoFormatterWidget(QWidget *parent = nullptr);
    ~TextProtoFormatterWidget();
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
    Ui::TextProtoFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // TEXTPROTOFORMATTERWIDGET_H
