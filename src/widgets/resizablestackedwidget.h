#ifndef RESIZABLESTACKEDWIDGET_H
#define RESIZABLESTACKEDWIDGET_H

#include <QStackedWidget>

class ResizableStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ResizableStackedWidget(QWidget* parent = nullptr);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};

#endif // RESIZABLESTACKEDWIDGET_H
