#include "apitester.h"
#include "src/widgets/ui_apitester.h"

ApiTester::ApiTester(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ApiTester)
{
    ui->setupUi(this);
    setName(tr("Api Tester"));
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ApiTester::onRequestFinished);
}

ApiTester::~ApiTester()
{
    delete ui;
}

bool ApiTester::canOpenFiles() const
{
    return true;
}

bool ApiTester::canSaveFiles() const
{
    return true;
}

bool ApiTester::canBasicEdit() const
{
    return true;
}

bool ApiTester::canChangeFont() const
{
    return true;
}

void ApiTester::save()
{

}

void ApiTester::saveAs()
{

}

void ApiTester::open()
{

}

void ApiTester::close()
{

}

void ApiTester::increaseFontSize()
{

}

void ApiTester::decreaseFontSize()
{

}

void ApiTester::setFontSize()
{

}

void ApiTester::resetFontSize()
{

}

void ApiTester::setFont()
{

}

QString ApiTester::getOpenedFileName() const
{
    return openedRequestFile + " " + openedResponseFile;
}

QStringList ApiTester::getRecentFiles() const
{
    return recentRequestFiles + recentResponseFiles;
}

void ApiTester::openFromRecent(const QString &path)
{

}

void ApiTester::clearRecent()
{

}

void ApiTester::sendGetRequest()
{

}

void ApiTester::sendPostRequest()
{

}

void ApiTester::sendPutRequest()
{

}

void ApiTester::onRequestFinished(QNetworkReply *reply)
{

}
