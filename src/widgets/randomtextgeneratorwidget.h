#ifndef RANDOMTEXTGENERATORWIDGET_H
#define RANDOMTEXTGENERATORWIDGET_H

#include <QWidget>
#include <QFutureWatcher>

namespace Ui {
class RandomTextGeneratorWidget;
}

class RandomTextGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RandomTextGeneratorWidget(QWidget *parent = nullptr);
    ~RandomTextGeneratorWidget();

private:
    Ui::RandomTextGeneratorWidget *ui;
    const QList<QChar> letters {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    const QList<QChar> numbers {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const QList<QChar> specialCharacters {'+', '-', '/', '*', '_', '=', '?', '.', ',', '>', '<', '\\', '[', ']', '{', '}', '!', '@', '#', '$', '%', '^', '&', '(', ')', '`', '~'};
    QFutureWatcher<QString> watcher;

private:
    void setUiEnabled(bool val);

private slots:
    void generate();
    void generationFinished();
};

#endif // RANDOMTEXTGENERATORWIDGET_H
