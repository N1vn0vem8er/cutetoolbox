#ifndef JAVASCRIPTFORMATTERWIDGET_H
#define JAVASCRIPTFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class JavaScriptFormatterWidget;
}

class JavaScriptFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit JavaScriptFormatterWidget(QWidget *parent = nullptr);
    ~JavaScriptFormatterWidget();
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
    Ui::JavaScriptFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // JAVASCRIPTFORMATTERWIDGET_H
