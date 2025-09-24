#ifndef MARKDOWNWIDGET_H
#define MARKDOWNWIDGET_H

#include "customwidget.h"
#include "document.h"
#include "syntaxhighlighters/markdownsyntaxhighlighter.h"
#include <QWidget>

namespace Ui {
class MarkdownWidget;
}

class MarkdownWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit MarkdownWidget(QWidget *parent = nullptr);
    ~MarkdownWidget();
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
    Ui::MarkdownWidget *ui;
    Document document;
    MarkdownSyntaxHighlighter* syntaxHighlighter {nullptr};
    QString openedFile;
};

#endif // MARKDOWNWIDGET_H
