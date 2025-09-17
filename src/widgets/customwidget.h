#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(QWidget *parent = nullptr);
    explicit CustomWidget(const QString& name, QWidget *parent = nullptr);
    virtual bool canOpenFiles() const;
    virtual bool canSaveFiles() const;
    virtual bool canBasicEdit() const;
    virtual bool canChangeFont() const;
    virtual void save();
    virtual void saveAs();
    virtual void open();
    virtual void increaseFontSize();
    virtual void decreaseFontSize();
    virtual void setFontSize();
    virtual void resetFontSize();
    virtual void setFont();
    virtual QString getName() const;
    void setName(const QString& name);

private:
    QString name;

signals:
    void saved(const QString& path);
};

#endif // CUSTOMWIDGET_H
