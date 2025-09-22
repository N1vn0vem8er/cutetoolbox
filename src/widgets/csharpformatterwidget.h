#ifndef CSHARPFORMATTERWIDGET_H
#define CSHARPFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class CSharpFormatterWidget;
}

class CSharpFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit CSharpFormatterWidget(QWidget *parent = nullptr);
    ~CSharpFormatterWidget();
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
    Ui::CSharpFormatterWidget *ui;
    QString openedFile;

private slots:
    void format();
};

#endif // CSHARPFORMATTERWIDGET_H
