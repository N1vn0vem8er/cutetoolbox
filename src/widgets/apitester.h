#ifndef APITESTER_H
#define APITESTER_H

#include "customwidget.h"
#include <QWidget>
#include <QNetworkAccessManager>

namespace Ui {
class ApiTester;
}

class ApiTester : public CustomWidget
{
    Q_OBJECT

public:
    explicit ApiTester(QWidget *parent = nullptr);
    ~ApiTester();
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
    Ui::ApiTester *ui;
    QString openedRequestFile;
    QString openedResponseFile;
    QStringList recentRequestFiles;
    QStringList recentResponseFiles;
    QNetworkAccessManager networkManager;

private slots:
    void sendGetRequest();
    void sendPostRequest();
    void sendPutRequest();
    void onRequestFinished(QNetworkReply* reply);
};

#endif // APITESTER_H
