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
    bool canOpenFiles() const override;
    void open() override;
    void close() override;
    QString getOpenedFileName() const override;

private:
    Ui::VideoConverterWidget *ui;
    QStringList decodingFormats;
    QStringList encodingFormats;
    QString openedFile;

private slots:
    void convert();

};

#endif // VIDEOCONVERTERWIDGET_H
