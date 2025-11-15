#include "apitesterwidget.h"
#include "src/widgets/ui_apitesterwidget.h"
#include <QStandardItemModel>
#include <QNetworkReply>
#include <qclipboard.h>
#include <qdialog.h>
#include <qjsonarray.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <QInputDialog>
#include <QFontDialog>
#include <QJsonDocument>
#include <QJsonObject>

ApiTesterWidget::ApiTesterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ApiTesterWidget)
{
    ui->setupUi(this);
    setName(tr("Api Tester"));
    ui->infoLabel->setVisible(false);
    connect(&networkManager, &QNetworkAccessManager::finished, this, &ApiTesterWidget::onRequestFinished);
    connect(ui->sendGetButton, &QPushButton::clicked, this, &ApiTesterWidget::sendGetRequest);
    connect(ui->addHeaderBotton, &QPushButton::clicked, this, &ApiTesterWidget::addHeader);
    connect(ui->removeHeaderButton, &QPushButton::clicked, this, &ApiTesterWidget::removeHeader);
    connect(ui->openRequestHeadersButton, &QPushButton::clicked, this, &ApiTesterWidget::openRequestHeaders);
    connect(ui->saveRequestHeaderButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.json");
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(QJsonDocument(headersToJson(static_cast<QStandardItemModel*>(ui->requestTableView->model()))).toJson());
                file.close();
            }
        }
    });
    connect(ui->saveResponesHeadersButton, &QPushButton::clicked, this, [&]{
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(), "*.json");
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(QJsonDocument(headersToJson(static_cast<QStandardItemModel*>(ui->responseTableView->model()))).toJson());
                file.close();
            }
        }
    });
    connect(ui->clearRequestHeadersButton, &QPushButton::clicked, this, [&]{
        if(ui->requestTableView->model())
            ui->requestTableView->model()->deleteLater();
        QStandardItemModel* model = new QStandardItemModel(ui->requestTableView);
        model->setHorizontalHeaderItem(0, new QStandardItem(tr("Header")));
        model->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
        ui->requestTableView->setModel(model);
    });
    connect(ui->copyRequestHeadersButton, &QPushButton::clicked, this, [&]{
        QGuiApplication::clipboard()->setText(QJsonDocument(headersToJson(static_cast<QStandardItemModel*>(ui->requestTableView->model()))).toJson());
    });
    connect(ui->copyResponseHeadersButton, &QPushButton::clicked, this, [&]{
        QGuiApplication::clipboard()->setText(QJsonDocument(headersToJson(static_cast<QStandardItemModel*>(ui->responseTableView->model()))).toJson());
    });
    connect(ui->pasteRequestHeadersButton, &QPushButton::clicked, this, &ApiTesterWidget::pasteRequestHeaders);
    connect(ui->copyButton, &QPushButton::clicked, this, [&]{ui->urlLineEdit->selectAll(); ui->urlLineEdit->copy();});
    connect(ui->pasteButton, &QPushButton::clicked, ui->urlLineEdit, &QLineEdit::paste);
    QStandardItemModel* model = new QStandardItemModel(ui->requestTableView);
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
    ui->requestTableView->setModel(model);
    QStandardItemModel* responseModel = new QStandardItemModel(ui->responseTableView);
    responseModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Header")));
    responseModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
    ui->responseTableView->setModel(responseModel);
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
    if(!openedRequestFile.isEmpty() || !openedResponseFile.isEmpty())
    {
        TextEdits option = TextEdits::none;
        if(ui->requestBody->hasFocus() && !openedRequestFile.isEmpty())
            option = TextEdits::request;
        else if(ui->responseBody->hasFocus() && !openedResponseFile.isEmpty())
            option = TextEdits::response;
        else if(!openedRequestFile.isEmpty() && !openedResponseFile.isEmpty())
            option = getSelectedOption();
        else if(!openedRequestFile.isEmpty())
            option = TextEdits::request;
        else if(openedResponseFile.isEmpty())
            option = TextEdits::response;
        if(option != TextEdits::none)
        {
            if(option == TextEdits::request)
            {
                QFile file(openedRequestFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->requestBody->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedRequestFile));
                }
            }
            else if(option == TextEdits::response)
            {
                QFile file(openedResponseFile);
                if(file.open(QIODevice::WriteOnly))
                {
                    file.write(ui->responseBody->toPlainText().toUtf8());
                    file.close();
                    emit saved(tr("Saved: %1").arg(openedResponseFile));
                }
            }
        }
    }
    else
        saveAs();
}

void ApiTesterWidget::saveAs()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                if(option == TextEdits::request)
                {
                    file.write(ui->requestBody->toPlainText().toUtf8());
                    openedRequestFile = path;
                    emit saved(tr("Saved: %1").arg(openedRequestFile));
                }
                else if(option == TextEdits::response)
                {
                    file.write(ui->responseBody->toPlainText().toUtf8());
                    openedResponseFile = path;
                    emit saved(tr("Saved: %1").arg(openedResponseFile));
                }
                file.close();
                emit opened(openedRequestFile + " " + openedResponseFile);
            }
        }
    }
}

void ApiTesterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Select Body"), QDir::homePath());
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            orginalFile = file.readAll();
            file.close();
            ui->requestBody->setPlainText(orginalFile);
            openedRequestFile = path;
            if(recentRequestFiles.length() >= 10)
                recentRequestFiles.removeFirst();
            if(!recentRequestFiles.contains(openedRequestFile))
                recentRequestFiles.append(openedRequestFile);
            emit updateRecent();
            emit opened(openedRequestFile);
        }
    }
}

void ApiTesterWidget::close()
{
    if(openedRequestFile.isEmpty())
        openedRequestFile.clear();
    else if(openedResponseFile.isEmpty())
        openedResponseFile.clear();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::request)
                openedRequestFile.clear();
            else if(option == TextEdits::response)
                openedResponseFile.clear();
        }
    }
    emit opened(openedRequestFile + " " + openedResponseFile);
}

void ApiTesterWidget::increaseFontSize()
{
    if(ui->requestBody->hasFocus())
        ui->requestBody->increaseFontSize();
    else if(ui->responseBody->hasFocus())
        ui->responseBody->increaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::request)
                ui->requestBody->increaseFontSize();
            else if(option == TextEdits::response)
                ui->responseBody->increaseFontSize();
        }
    }
}

void ApiTesterWidget::decreaseFontSize()
{
    if(ui->requestBody->hasFocus())
        ui->requestBody->decreaseFontSize();
    else if(ui->responseBody->hasFocus())
        ui->responseBody->decreaseFontSize();
    else
    {
        TextEdits option = getSelectedOption();
        if(option != TextEdits::none)
        {
            if(option == TextEdits::request)
                ui->requestBody->decreaseFontSize();
            else if(option == TextEdits::response)
                ui->responseBody->decreaseFontSize();
        }
    }
}

void ApiTesterWidget::setFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200);
        if(option == TextEdits::request)
            ui->requestBody->setFontSize(size);
        else if(option == TextEdits::response)
            ui->responseBody->setFontSize(size);
    }
}

void ApiTesterWidget::resetFontSize()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        if(option == TextEdits::request)
            ui->requestBody->setFontSize(10);
        else if(option == TextEdits::response)
            ui->responseBody->setFontSize(10);
    }
}

void ApiTesterWidget::setFont()
{
    TextEdits option = getSelectedOption();
    if(option != TextEdits::none)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, this);
        if(ok)
        {
            if(option == TextEdits::request)
                ui->requestBody->setFont(font);
            else if(option == TextEdits::response)
                ui->responseBody->setFont(font);
        }
    }
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
    if(recentRequestFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            orginalFile = file.readAll();
            file.close();
            ui->requestBody->setPlainText(orginalFile);
            openedRequestFile = path;
            emit opened(openedRequestFile + " " + openedResponseFile);
        }
    }
    else if(recentResponseFiles.contains(path))
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            orginalFile = file.readAll();
            file.close();
            ui->responseBody->setPlainText(orginalFile);
            openedResponseFile = path;
            emit opened(openedRequestFile + " " + openedResponseFile);
        }
    }
}

void ApiTesterWidget::clearRecent()
{
    recentRequestFiles.clear();
    emit updateRecent();
}

QJsonObject ApiTesterWidget::headersToJson(QStandardItemModel *model) const
{
    QJsonObject headers;
    for(int i = 0;i < model->rowCount(); i++)
    {
        headers[model->data(model->index(i, 0)).toString()] = model->data(model->index(i, 1)).toString();
    }
    return headers;
}

void ApiTesterWidget::setSendButtonsEnabled(bool val)
{
    ui->sendGetButton->setEnabled(val);
    ui->sendPostButton->setEnabled(val);
    ui->sendPutButton->setEnabled(val);
}

void ApiTesterWidget::setEditHeadersButtonsEnabled(bool val)
{
    ui->addHeaderBotton->setEnabled(val);
    ui->removeHeaderButton->setEnabled(val);
    ui->pasteRequestHeadersButton->setEnabled(val);
    ui->clearRequestHeadersButton->setEnabled(val);
    ui->openRequestHeadersButton->setEnabled(val);
}

ApiTesterWidget::TextEdits ApiTesterWidget::getSelectedOption()
{
    TextEdits option = TextEdits::none;
    QDialog dialog(this);
    QHBoxLayout layout(&dialog);
    QPushButton textButton(tr("Request"), &dialog);
    QPushButton base64Button(tr("Response"), &dialog);
    connect(&textButton, &QPushButton::clicked, &dialog, [&]{option = TextEdits::request;});
    connect(&base64Button, &QPushButton::clicked, &dialog, [&]{option = TextEdits::response;});
    connect(&textButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&base64Button, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout.addWidget(&textButton);
    layout.addWidget(&base64Button);
    dialog.setLayout(&layout);
    if(dialog.exec() == QDialog::Accepted)
    {
        return option;
    }
    return option;
}

void ApiTesterWidget::sendGetRequest()
{
    const QUrl url(ui->urlLineEdit->text());
    if(url.isValid())
    {
        QNetworkRequest request(url);
        QHttpHeaders headers;
        QAbstractItemModel* model = ui->requestTableView->model();
        if(model)
            for(int i = 0;i < model->rowCount(); i++)
            {
                headers.append(model->data(model->index(i, 0)).toString(), model->data(model->index(i, 1)).toString());
            }
        request.setHeaders(headers);
        networkManager.get(request);
        setSendButtonsEnabled(false);
        setEditHeadersButtonsEnabled(false);
    }
}

void ApiTesterWidget::sendPostRequest()
{
    const QUrl url(ui->urlLineEdit->text());
    if(url.isValid())
    {
        QNetworkRequest request(url);
        QHttpHeaders headers;
        QAbstractItemModel* model = ui->requestTableView->model();
        if(model)
            for(int i = 0;i < model->rowCount(); i++)
            {
                headers.append(model->data(model->index(i, 0)).toString(), model->data(model->index(i, 1)).toString());
            }
        request.setHeaders(headers);
        networkManager.post(request, ui->requestBody->toPlainText().toUtf8());
        setSendButtonsEnabled(false);
        setEditHeadersButtonsEnabled(false);
    }
}

void ApiTesterWidget::sendPutRequest()
{
    const QUrl url(ui->urlLineEdit->text());
    if(url.isValid())
    {
        QNetworkRequest request(url);
        QHttpHeaders headers;
        QAbstractItemModel* model = ui->requestTableView->model();
        if(model)
            for(int i = 0;i < model->rowCount(); i++)
            {
                headers.append(model->data(model->index(i, 0)).toString(), model->data(model->index(i, 1)).toString());
            }
        request.setHeaders(headers);
        networkManager.put(request, ui->requestBody->toPlainText().toUtf8());
        setSendButtonsEnabled(false);
        setEditHeadersButtonsEnabled(false);
    }
}

void ApiTesterWidget::onRequestFinished(QNetworkReply *reply)
{
    ui->infoLabel->setVisible(true);
    switch(reply->error())
    {
    case QNetworkReply::NoError:
        ui->infoLabel->setVisible(false);
        break;
    case QNetworkReply::ConnectionRefusedError:
        ui->infoLabel->setText(tr("Connection Refused"));
        break;
    case QNetworkReply::RemoteHostClosedError:
        ui->infoLabel->setText(tr("Remote Host Closed"));
        break;
    case QNetworkReply::HostNotFoundError:
        ui->infoLabel->setText(tr("Host Not Found"));
        break;
    case QNetworkReply::TimeoutError:
        ui->infoLabel->setText(tr("Timeout"));
        break;
    case QNetworkReply::OperationCanceledError:
        ui->infoLabel->setText(tr("Operation Canceled"));
        break;
    case QNetworkReply::SslHandshakeFailedError:
        ui->infoLabel->setText(tr("Ssl Handshake Failed"));
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        ui->infoLabel->setText(tr("Temporary Network Failure"));
        break;
    case QNetworkReply::NetworkSessionFailedError:
        ui->infoLabel->setText(tr("Network Session Failed"));
        break;
    case QNetworkReply::BackgroundRequestNotAllowedError:
        ui->infoLabel->setText(tr("Background Request Not Allowed"));
        break;
    case QNetworkReply::TooManyRedirectsError:
        ui->infoLabel->setText(tr("Too Many Redirects"));
        break;
    case QNetworkReply::InsecureRedirectError:
        ui->infoLabel->setText(tr("Insecure Redirect"));
        break;
    case QNetworkReply::UnknownNetworkError:
        ui->infoLabel->setText(tr("Unknown Netword"));
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
        ui->infoLabel->setText(tr("Proxy Connection Refused"));
        break;
    case QNetworkReply::ProxyConnectionClosedError:
        ui->infoLabel->setText(tr("Proxy Connection Closed"));
        break;
    case QNetworkReply::ProxyNotFoundError:
        ui->infoLabel->setText(tr("Proxy Not Found"));
        break;
    case QNetworkReply::ProxyTimeoutError:
        ui->infoLabel->setText(tr("Proxy Timeout"));
        break;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        ui->infoLabel->setText(tr("Proxy Authentication Required"));
        break;
    case QNetworkReply::UnknownProxyError:
        ui->infoLabel->setText(tr("Unknown Proxy"));
        break;
    case QNetworkReply::ContentAccessDenied:
        ui->infoLabel->setText(tr("Content Access Denied"));
        break;
    case QNetworkReply::ContentOperationNotPermittedError:
        ui->infoLabel->setText(tr("Content Operation Not Permitted"));
        break;
    case QNetworkReply::ContentNotFoundError:
        ui->infoLabel->setText(tr("Content Not Found"));
        break;
    case QNetworkReply::AuthenticationRequiredError:
        ui->infoLabel->setText(tr("Authentication Required"));
        break;
    case QNetworkReply::ContentReSendError:
        ui->infoLabel->setText(tr("Content Re Send"));
        break;
    case QNetworkReply::ContentConflictError:
        ui->infoLabel->setText(tr("Content Conflict"));
        break;
    case QNetworkReply::ContentGoneError:
        ui->infoLabel->setText(tr("Content Gone"));
        break;
    case QNetworkReply::UnknownContentError:
        ui->infoLabel->setText(tr("Unknown Content"));
        break;
    case QNetworkReply::ProtocolUnknownError:
        ui->infoLabel->setText(tr("Protocol Unknown"));
        break;
    case QNetworkReply::ProtocolInvalidOperationError:
        ui->infoLabel->setText(tr("Protocol Invalid Operation"));
        break;
    case QNetworkReply::ProtocolFailure:
        ui->infoLabel->setText(tr("Protocol Failure"));
        break;
    case QNetworkReply::InternalServerError:
        ui->infoLabel->setText(tr("Interna Server Error"));
        break;
    case QNetworkReply::OperationNotImplementedError:
        ui->infoLabel->setText(tr("Operation Not Implemented"));
        break;
    case QNetworkReply::ServiceUnavailableError:
        ui->infoLabel->setText(tr("Service Unavailable"));
        break;
    case QNetworkReply::UnknownServerError:
        ui->infoLabel->setText(tr("Unknown Server"));
        break;
    }
    if(ui->responseTableView->model())
        ui->responseTableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->responseTableView);
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
    const auto pairs = reply->headers().toListOfPairs();
    for(const auto& i : pairs)
    {
        model->appendRow({new QStandardItem(i.first), new QStandardItem(i.second)});
    }
    ui->responseTableView->setModel(model);
    ui->responseBody->setPlainText(reply->readAll());
    setSendButtonsEnabled(true);
    setEditHeadersButtonsEnabled(true);
}

void ApiTesterWidget::addHeader()
{
    if(ui->requestTableView->model())
    {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Add Header"));
        QVBoxLayout layout;
        QLineEdit name;
        name.setPlaceholderText(tr("Header name"));
        QLineEdit value;
        value.setPlaceholderText(tr("Header value"));
        layout.addWidget(&name);
        layout.addWidget(&value);
        QPushButton okButton(tr("Add"));
        QPushButton cancelButton(tr("Cancel"));
        connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        layout.addWidget(&okButton);
        layout.addWidget(&cancelButton);
        dialog.setLayout(&layout);
        if(dialog.exec() == QDialog::Accepted)
        {
            QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->requestTableView->model());
            model->appendRow({new QStandardItem(name.text()), new QStandardItem(value.text())});
        }
    }
}

void ApiTesterWidget::removeHeader()
{
    if(ui->requestTableView->selectionModel()->hasSelection())
    {
        QModelIndex index = ui->requestTableView->selectionModel()->selectedIndexes()[0];
        if(QMessageBox::question(this, tr("Remove"), tr("Remove header?")) == QMessageBox::Yes)
        {
            ui->requestTableView->model()->removeRow(index.row());
        }
    }
}

void ApiTesterWidget::openRequestHeaders()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), "*.json");
    if(!path.isEmpty())
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly))
        {
            QJsonParseError parseError;
            QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
            file.close();
            if(parseError.error == QJsonParseError::NoError)
            {
                QJsonObject obj = document.object();
                const auto objs = obj.keys();
                for(const auto& header : objs)
                {
                    QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->requestTableView->model());
                    model->appendRow({new QStandardItem(header), new QStandardItem(obj.value(header).toString())});
                }
                ui->infoLabel->clear();
                ui->infoLabel->setVisible(false);
            }
            else
            {
                ui->infoLabel->setText(parseError.errorString());
                ui->infoLabel->setVisible(true);
            }
        }
    }
}

void ApiTesterWidget::pasteRequestHeaders()
{
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(QGuiApplication::clipboard()->text().toUtf8(), &parseError);
    if(parseError.error == QJsonParseError::NoError)
    {
        QJsonObject obj = document.object();
        const auto objs = obj.keys();
        for(const auto& header : objs)
        {
            QStandardItemModel* model = static_cast<QStandardItemModel*>(ui->requestTableView->model());
            model->appendRow({new QStandardItem(header), new QStandardItem(obj.value(header).toString())});
        }
        ui->infoLabel->clear();
        ui->infoLabel->setVisible(false);
    }
    else
    {
        ui->infoLabel->setText(tr("Parsing error &1").arg(parseError.errorString()));
        ui->infoLabel->setVisible(true);
    }
}
