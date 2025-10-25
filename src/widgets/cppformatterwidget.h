#ifndef CPPFORMATTERWIDGET_H
#define CPPFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class CppFormatterWidget;
}

class CppFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit CppFormatterWidget(QWidget *parent = nullptr);
    ~CppFormatterWidget();
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
    Ui::CppFormatterWidget *ui;
    QString openedFile;
    QStringList recentFiles;

private slots:
    void format();
};

#endif // CPPFORMATTERWIDGET_H
