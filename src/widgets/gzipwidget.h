#ifndef GZIPWIDGET_H
#define GZIPWIDGET_H

#include <QWidget>

namespace Ui {
class GZipWidget;
}

class GZipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GZipWidget(QWidget *parent = nullptr);
    ~GZipWidget();

private:
    Ui::GZipWidget *ui;
    bool compressing {false};

private slots:
    void compress();
    void decompress();
};

#endif // GZIPWIDGET_H
