#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include "customwidget.h"
#include <QWidget>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qgraphicsscene.h>
#include <qlineedit.h>

namespace Ui {
class QrCodeGenerator;
}

class QrCodeGenerator : public CustomWidget
{
    Q_OBJECT

public:
    explicit QrCodeGenerator(QWidget *parent = nullptr);
    ~QrCodeGenerator();
    bool canSaveFiles() const override;
    void save() override;
    void saveAs() override;

private:
    Ui::QrCodeGenerator *ui;
    QLineEdit* textUrl {nullptr};
    QGraphicsScene* scene {nullptr};
    QWidget* textUrlWidget {nullptr};
    QWidget* wifiNetworkWidget {nullptr};
    QLineEdit* networkName {nullptr};
    QComboBox* encryptionType {nullptr};
    QLineEdit* networkPassword {nullptr};
    QWidget* contactWidget {nullptr};
    QLineEdit* firstName {nullptr};
    QLineEdit* lastName {nullptr};
    QLineEdit* phoneNumber {nullptr};
    QLineEdit* email {nullptr};

private slots:
    void generate();
    void optionSelected(int index);

protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // QRCODEGENERATOR_H
