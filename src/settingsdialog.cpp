#include "settingsdialog.h"
#include "src/ui_settingsdialog.h"
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    restoreGeometry(settings.value("settingsDialog.geometry").toByteArray());
    ui->focusSearchcheckBox->setChecked(settings.value("search.focusOnOpen").toBool());
    ui->clearSearchCheckBox->setChecked(settings.value("search.clearOnClose", false).toBool());
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
}
