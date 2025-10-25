#ifndef GZIPWIDGET_H
#define GZIPWIDGET_H

#include "customwidget.h"
#include <QWidget>
#include <qfuturewatcher.h>

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
        input,
        output,
        none
    };
    Ui::GZipWidget *ui;
    bool compressing {false};
    QString openedInputFile;
    QString openedOutputFile;
    QFutureWatcher<QByteArray> compressingWatcher;
    QFutureWatcher<QByteArray> decompressingWatcher;
    QStringList recentInputFiles;
    QStringList recentOutputFiles;

private:
    GZipWidget::TextEdits getSelectedOption();

private slots:
    void compress();
    void decompress();
};

#endif // GZIPWIDGET_H
