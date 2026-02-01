#include "settingsdialog.h"
#include "src/ui_settingsdialog.h"
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    restoreGeometry(settings.value("settingsDialog.geometry").toByteArray());
    ui->focusSearchcheckBox->setChecked(settings.value("search.focusOnOpen", true).toBool());
    ui->clearSearchCheckBox->setChecked(settings.value("search.clearOnClose", false).toBool());
    ui->saveGeometryCheckBox->setChecked(settings.value("save.geometry", true).toBool());
    ui->saveStateCheckBox->setChecked(settings.value("save.state", true).toBool());
    ui->openHtmlComboBox->setCurrentIndex(settings.value("open.html") == "formatter" ? 0 : 1);
    ui->openCppComboBox->setCurrentIndex(settings.value("open.cpp") == "formatter" ? 0 : 1);
    connect(ui->applyButton, &QPushButton::clicked, this, &SettingsDialog::apply);
}

SettingsDialog::~SettingsDialog()
{
    settings.setValue("settingsDialog.geometry", saveGeometry());
    delete ui;
}

void SettingsDialog::apply()
{
    settings.setValue("search.focusOnOpen", ui->focusSearchcheckBox->isChecked());
    settings.setValue("search.clearOnClose", ui->clearSearchCheckBox->isChecked());
    settings.setValue("save.state", ui->saveStateCheckBox->isChecked());
    settings.setValue("save.geometry", ui->saveGeometryCheckBox->isChecked());
    settings.setValue("open.html", ui->openHtmlComboBox->currentIndex() == 0 ? "formatter" : "encoder");
    settings.setValue("open.cpp", ui->openCppComboBox->currentIndex() == 0 ? "formatter" : "removecomments");
}
