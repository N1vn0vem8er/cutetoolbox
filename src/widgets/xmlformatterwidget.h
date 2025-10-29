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

private slots:
    void format();

private:
    Ui::XMLFormatterWidget *ui;
    QString openedFile;
    QStringList recentFiles;
};

#endif // XMLFORMATTERWIDGET_H
