#ifndef LOREMIPSUMGENERATOR_H
#define LOREMIPSUMGENERATOR_H

#include <QWidget>

namespace Ui {
class LoremIpsumGenerator;
}

class LoremIpsumGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit LoremIpsumGenerator(QWidget *parent = nullptr);
    ~LoremIpsumGenerator();

private:
    Ui::LoremIpsumGenerator *ui;
};

#endif // LOREMIPSUMGENERATOR_H
