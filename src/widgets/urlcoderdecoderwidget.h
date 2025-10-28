#ifndef URLCODERDECODERWIDGET_H
#define URLCODERDECODERWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class UrlCoderDecoderWidget;
}

class UrlCoderDecoderWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit UrlCoderDecoderWidget(QWidget *parent = nullptr);
    ~UrlCoderDecoderWidget();
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
    QStringList getRecentFiles() const override;
    void openFromRecent(const QString &path) override;
    void clearRecent() override;

private:
    enum TextEdits{
        decoded,
        encoded,
        none
    };
    Ui::UrlCoderDecoderWidget *ui;
    bool coding {false};
    QString openedEncodedFile;
    QString openedDecodedFile;
    QStringList recentEncodedFiles;
    QStringList recentDecodedFiles;

private:
    TextEdits getSelectedOption();

private slots:
    void encode();
    void decode();
};

#endif // URLCODERDECODERWIDGET_H
