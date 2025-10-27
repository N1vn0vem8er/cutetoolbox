#ifndef OBJECTIVECFORMATTERWIDGET_H
#define OBJECTIVECFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ObjectiveCFormatterWidget;
}

class ObjectiveCFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ObjectiveCFormatterWidget(QWidget *parent = nullptr);
    ~ObjectiveCFormatterWidget();
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
    Ui::ObjectiveCFormatterWidget *ui;
    QString openedFile;
    QStringList recentFiles;

private slots:
    void format();
};

#endif // OBJECTIVECFORMATTERWIDGET_H
