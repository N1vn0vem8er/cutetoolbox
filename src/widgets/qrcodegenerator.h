#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QWidget>
#include <qformlayout.h>
#include <qgraphicsscene.h>
#include <qlineedit.h>

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
    QFormLayout* textUrlLayout {nullptr};
    QLineEdit* textUrl {nullptr};
    QGraphicsScene* scene {nullptr};

private slots:
    void generate();
    void optionSelected();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // QRCODEGENERATOR_H
