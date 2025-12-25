#include "passwordverifierwidget.h"
#include "src/widgets/ui_passwordverifierwidget.h"

#include <QStringListModel>

PasswordVerifierWidget::PasswordVerifierWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::PasswordVerifierWidget)
{
    ui->setupUi(this);
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, &PasswordVerifierWidget::checkPassword);
    connect(ui->numbersCheckBox, &QCheckBox::clicked, this, &PasswordVerifierWidget::checkPassword);
    ui->listView->setStyleSheet("background-color: transparent; border: none;");
}

PasswordVerifierWidget::~PasswordVerifierWidget()
{
    delete ui;
}

bool PasswordVerifierWidget::canBasicEdit() const
{
    return true;
}

void PasswordVerifierWidget::checkPassword()
{
    const QString password = ui->passwordLineEdit->text();
    const int length = password.length();
    ui->lengthLabel->setText(QString::number(length));
    if(length == 0)
    {
        ui->passwordLineEdit->setPalette(QApplication::palette(ui->passwordLineEdit));
        return;
    }
    QStringList errors;
    const static QRegularExpression numberRegex(R"(\d+)");
    if(ui->numbersCheckBox->isChecked() && !numberRegex.match(password).hasMatch())
    {
        errors.append(tr("Password doesn't have numbers"));
    }
    if(length < ui->minLengthSpinBox->value())
    {
        errors.append(tr("Password is too short"));
    }
    if(length > ui->maxLengthSpinBox->value())
    {
        errors.append(tr("Password is too long"));
    }
    if(!errors.isEmpty())
    {
        QPalette palette = ui->passwordLineEdit->palette();
        palette.setColor(QPalette::Base, Qt::red);
        palette.setColor(QPalette::Text, Qt::white);
        ui->passwordLineEdit->setPalette(palette);
    }
    else
    {
        QPalette palette = ui->passwordLineEdit->palette();
        palette.setColor(QPalette::Base, Qt::green);
        palette.setColor(QPalette::Text, Qt::black);
        ui->passwordLineEdit->setPalette(palette);
    }
    if(ui->listView->model())
        ui->listView->model()->deleteLater();
    QStringListModel* model = new QStringListModel(ui->listView);
    model->setStringList(errors);
    ui->listView->setModel(model);
}
