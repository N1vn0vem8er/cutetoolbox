#ifndef GZIPWIDGET_H
#define GZIPWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class GZipWidget;
}

class GZipWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit GZipWidget(QWidget *parent = nullptr);
    ~GZipWidget();
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
    enum TextEdits{
        input,
        output,
        none
    };
    Ui::GZipWidget *ui;
    bool compressing {false};
    GZipWidget::TextEdits getSelectedOption();

private slots:
    void compress();
    void decompress();
};

#endif // GZIPWIDGET_H
