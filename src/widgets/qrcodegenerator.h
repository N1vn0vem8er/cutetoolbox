#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QWidget>

namespace Ui {
class QrCodeGenerator;
}

class QrCodeGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit QrCodeGenerator(QWidget *parent = nullptr);
    ~QrCodeGenerator();

private:
    Ui::QrCodeGenerator *ui;
};

#endif // QRCODEGENERATOR_H
