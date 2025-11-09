#include "apitesterwidget.h"
#include "src/widgets/ui_apitesterwidget.h"
#include <QStandardItemModel>
#include <QNetworkReply>
#include <qdialog.h>
#include <qmessagebox.h>

ApiTesterWidget::ApiTesterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ApiTesterWidget)
{
    ui->setupUi(this);
    setName(tr("Api Tester"));
    connect(&networkManager, &QNetworkAccessManager::finished, this, &ApiTesterWidget::onRequestFinished);
    connect(ui->sendGetButton, &QPushButton::clicked, this, &ApiTesterWidget::sendGetRequest);
    connect(ui->addHeaderBotton, &QPushButton::clicked, this, &ApiTesterWidget::addHeader);
    connect(ui->removeHeaderButton, &QPushButton::clicked, this, &ApiTesterWidget::removeHeader);
    QStandardItemModel* model = new QStandardItemModel(ui->requestTableView);
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
    ui->requestTableView->setModel(model);
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
    }
}

void ApiTesterWidget::onRequestFinished(QNetworkReply *reply)
{
    if(ui->responseTableView->model())
        ui->responseTableView->model()->deleteLater();
    QStandardItemModel* model = new QStandardItemModel(ui->responseTableView);
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
    for(const auto& i : reply->headers().toListOfPairs())
    {
        model->appendRow({new QStandardItem(i.first), new QStandardItem(i.second)});
    }
    ui->responseTableView->setModel(model);
    ui->responseBody->setPlainText(reply->readAll());
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
