#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customwidget.h"
#include <QMainWindow>
#include <qlabel.h>

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

private:
    Ui::MainWindow *ui;
    QMap<QString, int> menuIndexMap;
    QMenu* currentMenu {nullptr};
    QLabel* openedFileLabel {nullptr};

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
