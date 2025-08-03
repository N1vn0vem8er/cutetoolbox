#ifndef XMLFORMATTERWIDGET_H
#define XMLFORMATTERWIDGET_H

#include <QWidget>

namespace Ui {
class XMLFormatterWidget;
}

class XMLFormatterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XMLFormatterWidget(QWidget *parent = nullptr);
    ~XMLFormatterWidget();

private slots:
    void open();
    void format();

private:
    Ui::XMLFormatterWidget *ui;
};

#endif // XMLFORMATTERWIDGET_H
