#ifndef TEXTDIFFWIDGET_H
#define TEXTDIFFWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class TextDiffWidget;
}

class TextDiffWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit TextDiffWidget(QWidget *parent = nullptr);
    ~TextDiffWidget();
    bool canOpenFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void open() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;

private:
    enum TextEdits{
        oldText,
        newText,
        diff,
        none
    };
    Ui::TextDiffWidget *ui;
    TextEdits getSelectedOption();

private slots:
    void showDiff();
};

#endif // TEXTDIFFWIDGET_H
