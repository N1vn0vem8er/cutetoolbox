#ifndef DESKTOPCREATORWIDGET_H
#define DESKTOPCREATORWIDGET_H

#include <QWidget>

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
};

#endif // DESKTOPCREATORWIDGET_H
