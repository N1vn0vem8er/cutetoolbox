#include "passwordverifierwidget.h"
#include "config.h"
#include "src/widgets/ui_passwordverifierwidget.h"
#include <QStringListModel>
#include <qsettings.h>

PasswordVerifierWidget::PasswordVerifierWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::PasswordVerifierWidget)
{
    ui->setupUi(this);
    setName(tr("Password Verifier"));
    QSettings settings(Config::settingsName);
    ui->minLengthSpinBox->setValue(settings.value("passwordVerifier.minLength", 0).toInt());
    ui->maxLengthSpinBox->setValue(settings.value("passwordVerifier.maxLength", 10).toInt());
    ui->lettersSizeCheckBox->setChecked(settings.value("passwordVerifier.lettersSize", false).toBool());
    ui->numbersCheckBox->setChecked(settings.value("passwordVerifier.numbers", false).toBool());
    ui->specialCharscheckBox->setChecked(settings.value("passwordVerifier.specialCharacters", false).toBool());
    connect(ui->passwordLineEdit, &QLineEdit::textChanged, this, &PasswordVerifierWidget::checkPassword);
    connect(ui->numbersCheckBox, &QCheckBox::clicked, this, &PasswordVerifierWidget::checkPassword);
    connect(ui->specialCharscheckBox, &QCheckBox::clicked, this, &PasswordVerifierWidget::checkPassword);
    connect(ui->lettersSizeCheckBox, &QCheckBox::clicked, this, &PasswordVerifierWidget::checkPassword);
    ui->listView->setStyleSheet("background-color: transparent; border: none;");
}

PasswordVerifierWidget::~PasswordVerifierWidget()
{
    QSettings settings(Config::settingsName);
    settings.setValue("passwordVerifier.minLength", ui->minLengthSpinBox->value());
    settings.setValue("passwordVerifier.maxLength", ui->maxLengthSpinBox->value());
    settings.setValue("passwordVerifier.lettersSize", ui->lettersSizeCheckBox->isChecked());
    settings.setValue("passwordVerifier.numbers", ui->numbersCheckBox->isChecked());
    settings.setValue("passwordVerifier.specialCharacters", ui->specialCharscheckBox->isChecked());
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
    const static QRegularExpression specialCharacterRegex(R"([!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?~])");
    if(ui->specialCharscheckBox->isChecked() && !specialCharacterRegex.match(password).hasMatch())
    {
        errors.append(tr("Password doesn't have special characters"));
    }
    const static QRegularExpression lettersSizeRegex(R"((?=.*[a-z])(?=.*[A-Z]).+)");
    if(ui->lettersSizeCheckBox->isChecked() && !lettersSizeRegex.match(password).hasMatch())
    {
        errors.append(tr("Password doesn't have different size letters"));
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
