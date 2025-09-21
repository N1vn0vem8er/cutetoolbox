#include "videoconverterwidget.h"
#include "src/widgets/ui_videoconverterwidget.h"
#include <QFileDialog>
#include <qprocess.h>

VideoConverterWidget::VideoConverterWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::VideoConverterWidget)
{
    ui->setupUi(this);
    setName(tr("Video Format Converter"));
    connect(ui->openButton, &QPushButton::clicked, this, &VideoConverterWidget::open);
    QProcess* process = new QProcess(this);
    connect(process, &QProcess::finished, process, &QProcess::deleteLater);
    connect(process, &QProcess::readyReadStandardOutput, this, [this, process]{
        const QString output = process->readAllStandardOutput();
        QRegularExpressionMatchIterator iterator = QRegularExpression(R"(DE?\s+([a-z0-9_,]+))").globalMatch(output);
        while(iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            decodingFormats.append(match.captured(1));
        }
        QRegularExpressionMatchIterator iterator2 = QRegularExpression(R"(D?E\s+([a-z0-9_,]+))").globalMatch(output);
        while(iterator2.hasNext())
        {
            QRegularExpressionMatch match = iterator2.next();
            encodingFormats.append(match.captured(1));
        }
        for(const QString& i : std::as_const(decodingFormats))
        {
            if(i.contains(","))
            {
                decodingFormats.append(i.split(","));
                decodingFormats.removeAll(i);
            }
        }
        decodingFormats.removeDuplicates();
        for(const QString& i : std::as_const(encodingFormats))
        {
            if(i.contains(","))
            {
                encodingFormats.append(i.split(","));
                encodingFormats.removeAll(i);
            }
        }
        encodingFormats.removeDuplicates();
        ui->formatComboBox->addItems(encodingFormats);
    });
    process->start("ffmpeg", {"-formats"});
    process->waitForStarted();
}

VideoConverterWidget::~VideoConverterWidget()
{
    delete ui;
}

bool VideoConverterWidget::canOpenFiles() const
{
    return true;
}

void VideoConverterWidget::open()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath(), [this]{
        QString out;
        for(const QString& i : std::as_const(decodingFormats))
            out.append(QString("*.%1;;").arg(i));
        return out;}());
    if(!path.isEmpty())
    {

    }
}
