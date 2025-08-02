#ifndef CodeEditor_H
#define CodeEditor_H
#include <QPlainTextEdit>
#include <QCompleter>
#include <qlabel.h>
#include <qstringlistmodel.h>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);
    CodeEditor(const QString& text, const QString& path, QWidget* parent = nullptr);
    ~CodeEditor();
    QString getPath() const;
    void setPath(const QString &newPath);
    int lineNumberWidth();
    void lineNumberAreaPaint(QPaintEvent* event);
    QString getName() const;
    void setName(const QString &newName);
    void find(const QString& text);
    void replace(const QString& find, const QString& replace);
    void clearSearchFormatting();
    void deleteSelected();
    void deleteAll();
    void mergeSelectedLines();
    void makeSelectedSmall();
    void makeSelectedCapital();
    void makeSelectedSentenceCapital();
    void makeEverySelectedCapital();
    void increaseFontSize();
    void decreaseFontSize();
    void setFontSize(int size);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void dropEvent(QDropEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QString path;
    QWidget *lineNumberArea;
    QString name;
    void init();
    void updateLineNumberWidth(int count);
    void updateLineNumber(const QRect &rect, int dy);
    QTextCharFormat defaultFormat;
    QCompleter* completer {nullptr};
    QStringListModel* completerModel {nullptr};

private slots:
    void insertCompletion(const QModelIndex& index);

signals:
    void fontSizeChanged(int size);

};

#endif // CodeEditor_H
