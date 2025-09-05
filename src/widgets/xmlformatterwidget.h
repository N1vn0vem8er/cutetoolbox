#ifndef XMLFORMATTERWIDGET_H
#define XMLFORMATTERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class XMLFormatterWidget;
}

class XMLFormatterWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit XMLFormatterWidget(QWidget *parent = nullptr);
    ~XMLFormatterWidget();
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

private slots:
    void format();

private:
    Ui::XMLFormatterWidget *ui;
    QString openedFile;
};

#endif // XMLFORMATTERWIDGET_H
