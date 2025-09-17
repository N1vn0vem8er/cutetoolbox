#ifndef TABLEGENFORMATTERWIDGET_H
#define TABLEGENFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class TableGenFormatterWidget;
}

class TableGenFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit TableGenFormatterWidget(QWidget *parent = nullptr);
    ~TableGenFormatterWidget();
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

private:
    Ui::TableGenFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // TABLEGENFORMATTERWIDGET_H
