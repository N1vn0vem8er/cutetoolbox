#include "removecommentswidget.h"
#include "src/widgets/ui_removecommentswidget.h"

#include <QFontDialog>
#include <qinputdialog.h>

RemoveCommentsWidget::RemoveCommentsWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::RemoveCommentsWidget)
{
    ui->setupUi(this);
}

RemoveCommentsWidget::~RemoveCommentsWidget()
{
    delete ui;
}

bool RemoveCommentsWidget::canOpenFiles() const
{
    return true;
}

bool RemoveCommentsWidget::canSaveFiles() const
{
    return true;
}

bool RemoveCommentsWidget::canBasicEdit() const
{
    return true;
}

bool RemoveCommentsWidget::canChangeFont() const
{
    return true;
}

void RemoveCommentsWidget::save()
{

}

void RemoveCommentsWidget::saveAs()
{

}

void RemoveCommentsWidget::open()
{

}

void RemoveCommentsWidget::close()
{

}

void RemoveCommentsWidget::increaseFontSize()
{
    ui->codeEditor->increaseFontSize();
}

void RemoveCommentsWidget::decreaseFontSize()
{
    ui->codeEditor->decreaseFontSize();
}

void RemoveCommentsWidget::setFontSize()
{
    bool ok;
    const int size = QInputDialog::getInt(this, tr("Set font size"), tr("Font size"), 1, 1, 200, 1, &ok);
    if(ok)
        ui->codeEditor->setFontSize(size);
}

void RemoveCommentsWidget::resetFontSize()
{
    ui->codeEditor->setFontSize(10);
}

void RemoveCommentsWidget::setFont()
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, this);
    if(ok)
    {
        ui->codeEditor->setFont(font);
    }
}

QString RemoveCommentsWidget::getOpenedFileName() const
{

}

QStringList RemoveCommentsWidget::getRecentFiles() const
{

}

void RemoveCommentsWidget::openFromRecent(const QString &path)
{

}

void RemoveCommentsWidget::clearRecent()
{

}
