#ifndef CPPFORMATTERWIDGET_H
#define CPPFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class CppFormatterWidget;
}

class CppFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit CppFormatterWidget(QWidget *parent = nullptr);
    ~CppFormatterWidget();
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
    Ui::CppFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // CPPFORMATTERWIDGET_H
