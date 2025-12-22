#ifndef REMOVECOMMENTSWIDGET_H
#define REMOVECOMMENTSWIDGET_H

#include "customwidget.h"
#include <QWidget>

namespace Ui {
class RemoveCommentsWidget;
}

class RemoveCommentsWidget : public CustomWidget
{
    Q_OBJECT

public:
    explicit RemoveCommentsWidget(QWidget *parent = nullptr);
    ~RemoveCommentsWidget();
    bool canOpenFiles() const override;
    bool canSaveFiles() const override;
    bool canBasicEdit() const override;
    bool canChangeFont() const override;
    void save() override;
    void saveAs() override;
    void open() override;
    void close() override;
    void increaseFontSize() override;
    void decreaseFontSize() override;
    void setFontSize() override;
    void resetFontSize() override;
    void setFont() override;
    QString getOpenedFileName() const override;
    QStringList getRecentFiles() const override;
    void openFromRecent(const QString &path) override;
    void clearRecent() override;

private:
    Ui::RemoveCommentsWidget *ui;
    QString openedFile;
    QStringList recentFiles;
    enum class Languages{
        None = -1,
        Cpp,
        C,
        ObjectiveC,
        Python
    };

private:
    void removeCommentsClang(const QString& fileName, const QList<const char*>& args);
    void removeCommentsPython();

private slots:
    void removeComments();
};

#endif // REMOVECOMMENTSWIDGET_H
