#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include "customwidget.h"
#include <QMainWindow>
#include <qcompleter.h>
#include <qlabel.h>
#include <qsettings.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showWidget(const QModelIndex &index);
    void widgetChanged();
    void addMenuItem(const QString& text, const QIcon& icon, CustomWidget* widget = nullptr);
    void showAbout();
    void save();
    void saveAs();
    void open();
    void close();
    void copy();
    void cut();
    void paste();
    void selectAll();
    void deleteSelected();
    void deleteAll();
    void undo();
    void redo();
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize();
    void resetFontSize();
    void setFont();
    void savedFile(const QString& path);
    void openedFile(const QString& path);
    void find(QString text);
    void updateRecent();
    void showSettings();

private:
    Ui::MainWindow *ui;
    QMap<QString, int> menuIndexMap;
    QMenu* currentMenu {nullptr};
    QLabel* openedFileLabel {nullptr};
    QCompleter* completer {nullptr};
    QStringList toolNames;
    QSettings settings{Config::settingsName};

private:
    void showByName(const QString& name);
    void openFromRecent(const QString& path);
    void clearRecent();
    void loadDefaultSettings();
    bool isDarkTheme(const QPalette& pal);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
