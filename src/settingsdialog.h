#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "config.h"
#include <QDialog>
#include <qsettings.h>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    QSettings settings{Config::settingsName};

private slots:
    void apply();
};

#endif // SETTINGSDIALOG_H
