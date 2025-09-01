#ifndef LOREMIPSUMGENERATOR_H
#define LOREMIPSUMGENERATOR_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class LoremIpsumGenerator;
}

class LoremIpsumGenerator : public CustomWidget
{
    Q_OBJECT

public:
    explicit LoremIpsumGenerator(QWidget *parent = nullptr);
    ~LoremIpsumGenerator();
    bool canSaveFiles() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;

private:
    Ui::LoremIpsumGenerator *ui;
    const QStringList words = {
        "lorem", "ipsum", "dolor", "sit", "amet", "consectetur",
        "adipiscing", "elit", "sed", "do", "eiusmod", "tempor",
        "incididunt", "ut", "labore", "et", "dolore", "magna", "aliqua",
        "ut", "enim", "ad", "minim", "veniam", "quis", "nostrud",
        "exercitation", "ullamco", "laboris", "nisi", "ut", "aliquip",
        "ex", "ea", "commodo", "consequat", "duis", "aute", "irure",
        "dolor", "in", "reprehenderit", "in", "voluptate", "velit",
        "esse", "cillum", "dolore", "eu", "fugiat", "nulla", "pariatur",
        "excepteur", "sint", "occaecat", "cupidatat", "non", "proident",
        "sunt", "in", "culpa", "qui", "officia", "deserunt", "mollit",
        "anim", "id", "est", "laborum"
    };

private:
    QString generateSentence(int minWords, int maxWords, bool makeFirstCharUpper = true) const;

private slots:
    void generate();

};

#endif // LOREMIPSUMGENERATOR_H
