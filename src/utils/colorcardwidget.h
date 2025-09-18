#ifndef COLORCARDWIDGET_H
#define COLORCARDWIDGET_H

#include <QWidget>

namespace Ui {
class ColorCardWidget;
}

class ColorCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorCardWidget(QWidget *parent = nullptr);
    ~ColorCardWidget();
    void setColor(const QColor& newColor);
    QColor getColor() const;
    bool isLocked() const;
    void update();

private:
    Ui::ColorCardWidget *ui;
    QColor color;
    bool locked {false};

private slots:
    void lockOrUnlock();
    void remove();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void removeColor(ColorCardWidget* widget);
};

#endif // COLORCARDWIDGET_H
