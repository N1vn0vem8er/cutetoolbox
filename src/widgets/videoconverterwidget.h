#ifndef VIDEOCONVERTERWIDGET_H
#define VIDEOCONVERTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class VideoConverterWidget;
}

class VideoConverterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit VideoConverterWidget(QWidget *parent = nullptr);
    ~VideoConverterWidget();

private:
    Ui::VideoConverterWidget *ui;
};

#endif // VIDEOCONVERTERWIDGET_H
