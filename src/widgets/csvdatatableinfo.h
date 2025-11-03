#ifndef CSVDATATABLEINFO_H
#define CSVDATATABLEINFO_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class CSVDataTableInfo;
}

class CSVDataTableInfo : public CustomWidget
{
    Q_OBJECT

public:
    explicit CSVDataTableInfo(QWidget *parent = nullptr);
    ~CSVDataTableInfo();
    bool canOpenFiles() const override;
    void open() override;
    void close() override;
    QString getOpenedFileName() const override;
    QStringList getRecentFiles() const override;
    void openFromRecent(const QString &path) override;
    void clearRecent() override;

private:
    Ui::CSVDataTableInfo *ui;
    QString openedFile;
    QStringList recentFiles;

private:
    void parseCsv(const QString& csv);
    QString toCsv() const;
};

#endif // CSVDATATABLEINFO_H
