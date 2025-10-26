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
    void close() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    QString getOpenedFileName() const override;
    QStringList getRecentFiles() const override;
    void openFromRecent(const QString &path) override;
    void clearRecent() override;

private:
    Ui::JsonFormatterWidget *ui;
    JSONSyntaxHighlighter* syntaxHighlighter {nullptr};
    QString openedFile;
    QStringList recentFiles;

private slots:
    void format();
};

#endif // JSONFORMATTERWIDGET_H
