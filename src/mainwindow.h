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

private:
    Ui::MainWindow *ui;
    QList<QPair<QString, QString>> listOption {{tr("JSON"), ""}, {tr("SQL"), ""}, {tr("XML"), ""}, {tr("HTML"), ""}, {tr("CSS"), ""}};
};
#endif // MAINWINDOW_H
