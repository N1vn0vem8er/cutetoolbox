#ifndef HASHWIDGET_H
#define HASHWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class HashWidget;
}

class HashWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit HashWidget(QWidget *parent = nullptr);
    ~HashWidget();
    bool canOpenFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    void open() override;
    void close() override;
    QString getOpenedFileName() const override;

private:
    enum TextEdits{
        input,
        output,
        none
    };
    Ui::HashWidget *ui;
    enum HashTypes{
        MD5,
        SHA1,
        SHA256,
        SHA512,
        SHA3_256,
        SHA3_512,
        KECCAK256,
        KECCAK512,
        BLAKE2b256,
        BLAKE2b512,
        BLAKE2s256
    };

    const QMap<HashTypes, QString> hashTypes{
        {HashTypes::MD5, "MD5"},
        {HashTypes::SHA1, "SHA1"},
        {HashTypes::SHA256, "SHA256"},
        {HashTypes::SHA512, "SHA512"},
        {HashTypes::SHA3_256, "SHA3-256"},
        {HashTypes::SHA3_512, "SHA3-512"},
        {KECCAK256, "KECCAK256"},
        {KECCAK512, "KECCAK512"},
        {BLAKE2b256, "BLAKE2b256"},
        {BLAKE2b512, "BLAKE2b512"},
        {BLAKE2s256, "BLAKE2s256"}
        };
    QString openedFile;
    TextEdits getSelectedOption();

private slots:
    void calculateHash();
};

#endif // HASHWIDGET_H
