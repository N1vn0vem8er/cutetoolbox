#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QMap<QString, int> menuIndexMap;
};
#endif // MAINWINDOW_H
