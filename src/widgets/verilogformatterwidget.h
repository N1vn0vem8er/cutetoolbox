#ifndef VERILOGFORMATTERWIDGET_H
#define VERILOGFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class VerilogFormatterWidget;
}

class VerilogFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit VerilogFormatterWidget(QWidget *parent = nullptr);
    ~VerilogFormatterWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void close() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    QString getOpenedFileName() const override;

private:
    Ui::VerilogFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // VERILOGFORMATTERWIDGET_H
