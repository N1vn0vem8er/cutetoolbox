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
    enum TextEdits{
        oldText,
        newText,
        diff,
        none
    };
    Ui::TextDiffWidget *ui;
    TextEdits getSelectedOption();
    QString openedOldFile;
    QString openedNewFile;
    QStringList recentOldFiles;
    QStringList recentNewFiles;

private slots:
    void showDiff();
};

#endif // TEXTDIFFWIDGET_H
