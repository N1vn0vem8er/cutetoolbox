#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void addMenuItem(const QString& text, const QIcon& icon, bool enabled = true);
    void showAbout();
    void saveAs();
    void open();
    void copy();
    void cut();
    void paste();
    void selectAll();
    void deleteSelected();
    void deleteAll();
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize();
    void resetFontSize();
    void setFont();

private:
    Ui::MainWindow *ui;
    QMap<QString, int> menuIndexMap;
    QLabel* currentToolLabel {nullptr};
};
#endif // MAINWINDOW_H
