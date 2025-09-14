#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget* parent = nullptr);
    void setImage(const QImage& image);

private:
    QGraphicsScene* scene {nullptr};
    QGraphicsItemGroup* group {nullptr};
    QGraphicsPixmapItem* item {nullptr};

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // IMAGEVIEW_H
