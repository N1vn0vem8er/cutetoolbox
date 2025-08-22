#ifndef DESKTOPCREATORWIDGET_H
#define DESKTOPCREATORWIDGET_H

#include <QWidget>
#include <qfuturewatcher.h>

namespace Ui {
class DesktopCreatorWidget;
}

class DesktopCreatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DesktopCreatorWidget(QWidget *parent = nullptr);
    ~DesktopCreatorWidget();

private:
    Ui::DesktopCreatorWidget *ui;
    QFutureWatcher<QList<QString>> watcher;
    QStringList icons;

private:
    void startSearchingForIcons();

private slots:
    void generate();
};

#endif // DESKTOPCREATORWIDGET_H
