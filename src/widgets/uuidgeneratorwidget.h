#ifndef UUIDGENERATORWIDGET_H
#define UUIDGENERATORWIDGET_H

#include <QWidget>

namespace Ui {
class UUIDGeneratorWidget;
}

class UUIDGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UUIDGeneratorWidget(QWidget *parent = nullptr);
    ~UUIDGeneratorWidget();

private:
    Ui::UUIDGeneratorWidget *ui;

private slots:
    void generate();
};

#endif // UUIDGENERATORWIDGET_H
