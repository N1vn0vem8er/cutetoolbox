#ifndef SQLFORMATTERWIDGET_H
#define SQLFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class SQLFormatterWidget;
}

class SQLFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit SQLFormatterWidget(QWidget *parent = nullptr);
    ~SQLFormatterWidget();
    void open() override;
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void copy() override;
    void cut() override;
    void paste() override;
    void selectAll() override;
    void deleteText() override;
    void deleteAllText() override;
    void undo() override;
    void redo() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;

private:
    Ui::SQLFormatterWidget *ui;

private slots:
    void format();
};

#endif // SQLFORMATTERWIDGET_H
