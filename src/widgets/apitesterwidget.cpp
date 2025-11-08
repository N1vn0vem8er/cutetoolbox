#include "apitesterwidget.h"
#include "src/widgets/ui_apitesterwidget.h"

#include <QNetworkReply>

ApiTesterWidget::ApiTesterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ApiTesterWidget)
{
    ui->setupUi(this);
    setName(tr("Api Tester"));
    connect(&networkManager, &QNetworkAccessManager::finished, this, &ApiTesterWidget::onRequestFinished);
    connect(ui->sendGetButton, &QPushButton::clicked, this, &ApiTesterWidget::sendGetRequest);
}

ApiTesterWidget::~ApiTesterWidget()
{
    delete ui;
}

bool ApiTesterWidget::canOpenFiles() const
{
    return true;
}

bool ApiTesterWidget::canSaveFiles() const
{
    return true;
}

bool ApiTesterWidget::canBasicEdit() const
{
    return true;
}

bool ApiTesterWidget::canChangeFont() const
{
    return true;
}

void ApiTesterWidget::save()
{

}

void ApiTesterWidget::saveAs()
{

}

void ApiTesterWidget::open()
{

}

void ApiTesterWidget::close()
{

}

void ApiTesterWidget::increaseFontSize()
{

}

void ApiTesterWidget::decreaseFontSize()
{

}

void ApiTesterWidget::setFontSize()
{

}

void ApiTesterWidget::resetFontSize()
{

}

void ApiTesterWidget::setFont()
{

}

QString ApiTesterWidget::getOpenedFileName() const
{
    return openedRequestFile + " " + openedResponseFile;
}

QStringList ApiTesterWidget::getRecentFiles() const
{
    return recentRequestFiles + recentResponseFiles;
}

void ApiTesterWidget::openFromRecent(const QString &path)
{

}

void ApiTesterWidget::clearRecent()
{

}

void ApiTesterWidget::sendGetRequest()
{
    const QUrl url(ui->urlLineEdit->text());
    if(url.isValid())
    {
        networkManager.get(QNetworkRequest(url));
    }
}

void ApiTesterWidget::sendPostRequest()
{

}

void ApiTesterWidget::sendPutRequest()
{

}

void ApiTesterWidget::onRequestFinished(QNetworkReply *reply)
{
    qDebug() << reply->readAll();
}
