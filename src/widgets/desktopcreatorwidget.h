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

private:
    Ui::DesktopCreatorWidget *ui;
    QFutureWatcher<QList<QString>> watcher;
    QStringList icons;
    QString openedFile;

private:
    void startSearchingForIcons();

private slots:
    void generate();
    void openDesktopFile();
    void saveDesktopFile();
};

#endif // DESKTOPCREATORWIDGET_H
