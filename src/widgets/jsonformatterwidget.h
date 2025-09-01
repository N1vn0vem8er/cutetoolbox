#ifndef JSONFORMATTERWIDGET_H
#define JSONFORMATTERWIDGET_H

#include "customwidget.h"
#include "syntaxhighlighters/jsonsyntaxhighlighter.h"
#include <QWidget>

namespace Ui {
class JsonFormatterWidget;
}

class JsonFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit JsonFormatterWidget(QWidget *parent = nullptr);
    ~JsonFormatterWidget();
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
    Ui::JsonFormatterWidget *ui;
    JSONSyntaxHighlighter* syntaxHighlighter {nullptr};

private slots:
    void format();
};

#endif // JSONFORMATTERWIDGET_H
