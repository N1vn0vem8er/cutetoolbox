#ifndef UUIDGENERATORWIDGET_H
#define UUIDGENERATORWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class UUIDGeneratorWidget;
}

class UUIDGeneratorWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit UUIDGeneratorWidget(QWidget *parent = nullptr);
    ~UUIDGeneratorWidget();
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    QString getOpenedFileName() const override;

private:
    Ui::UUIDGeneratorWidget *ui;
    QString openedFile;

private slots:
    void generate();
};

#endif // UUIDGENERATORWIDGET_H
