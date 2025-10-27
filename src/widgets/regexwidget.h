#ifndef REGEXWIDGET_H
#define REGEXWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class RegexWidget;
}

class RegexWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit RegexWidget(QWidget *parent = nullptr);
    ~RegexWidget();
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
    Ui::RegexWidget *ui;
    QString openedFile;
    QStringList recentFiles;

private slots:
    void regex();
};

#endif // REGEXWIDGET_H
