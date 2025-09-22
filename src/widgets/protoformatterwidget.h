#ifndef PROTOFORMATTERWIDGET_H
#define PROTOFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class ProtoFormatterWidget;
}

class ProtoFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit ProtoFormatterWidget(QWidget *parent = nullptr);
    ~ProtoFormatterWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    QString getOpenedFileName() const override;

private:
    Ui::ProtoFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // PROTOFORMATTERWIDGET_H
