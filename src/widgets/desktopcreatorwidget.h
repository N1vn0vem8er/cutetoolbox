#ifndef DESKTOPCREATORWIDGET_H
#define DESKTOPCREATORWIDGET_H

#include "customwidget.h"
#include <QWidget>
#include <qfuturewatcher.h>

namespace Ui {
class DesktopCreatorWidget;
}

class DesktopCreatorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit DesktopCreatorWidget(QWidget *parent = nullptr);
    ~DesktopCreatorWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void close() override;
    QString getOpenedFileName() const override;
    QStringList getRecentFiles() const override;
    void openFromRecent(const QString &path) override;
    void clearRecent() override;

private:
    Ui::DesktopCreatorWidget *ui;
    QFutureWatcher<QList<QString>> watcher;
    QStringList icons;
    QString openedFile;
    QStringList recentFiles;

private:
    void startSearchingForIcons();
    void readFile(const QString& path);

private slots:
    void generate();
};

#endif // DESKTOPCREATORWIDGET_H
