#ifndef IMAGEFORMATCONVERTERWIDGET_H
#define IMAGEFORMATCONVERTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ImageFormatConverterWidget;
}

class ImageFormatConverterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ImageFormatConverterWidget(QWidget *parent = nullptr);
    ~ImageFormatConverterWidget();
    bool canOpenFiles() const override;
    void open() override;
    void close() override;
    QString getOpenedFileName() const override;

private:
    Ui::ImageFormatConverterWidget *ui;
    QImage image;
    QString openedFile;

private slots:
    void convert();
};

#endif // IMAGEFORMATCONVERTERWIDGET_H
